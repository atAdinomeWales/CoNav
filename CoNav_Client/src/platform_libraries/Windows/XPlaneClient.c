#include "XPlaneClient.h"
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>


static SOCKET udp_socket = INVALID_SOCKET;
static struct sockaddr_in xplane_address;
static bool winsock_started = false;

static float current_heading = 0.0f;
static bool heading_valid = false;

static float pending_heading = 0.0f;
static bool heading_pending = false;
static DWORD pending_since = 0;

bool XPlaneClient_adjst_rref(char* dataref, int direction){
    if (!dataref){
        return false;
    }
    if (direction == 0){
        return true;
    }

    if(!strcmp(dataref, HEADING_DATAREF)){
        return XPlaneClient_adjust_heading(direction);
    }

}

static bool send_packet(const char *packet, int length){
    if (udp_socket == INVALID_SOCKET) {
        return false;
    }

    return sendto(
        udp_socket,
        packet,
        length,
        0,
        (struct sockaddr *)&xplane_address,
        sizeof(xplane_address)
    ) != SOCKET_ERROR;
}


static float normalize_heading(float heading){
    while (heading >= 360.0f){
        heading -= 360.0f;
    }

    while (heading < 0.0f) {
        heading += 360.0f;
    }

    return heading;
}

static float heading_difference(float a, float b){
    float difference = normalize_heading(a - b);

    if (difference > 180.0f) {
        difference -= 360.0f;
    }

    return difference;
}


static bool request_heading(void){
    char packet[RREF_PACKET_SIZE] = {0};

    int32_t frequency = RREF_RATE_FAST;
    int32_t index = RREFID_HEADING;

    memcpy(packet, "RREF", 4);
    packet[4] = '\0';

    memcpy(packet + 5, &frequency, sizeof(frequency));
    memcpy(packet + 9, &index, sizeof(index));

    memcpy(packet + 13, HEADING_DATAREF, strlen(HEADING_DATAREF));

    return send_packet(packet, sizeof(packet));
}

static bool write_heading(float heading){
    char packet[DREF_PACKET_SIZE] = {0};

    memcpy(packet, "DREF", 4);
    packet[4] = '\0';

    memcpy(packet + 5, &heading, sizeof(heading));
    memcpy(packet + 9, HEADING_DATAREF, sizeof(HEADING_DATAREF));

    return send_packet(packet, sizeof(packet));
}

static bool XPlaneClient_init_subscriptions(void){
    if (!request_heading()) {
        printf("Failed to request heading dataref.\n");
        return false;
    }

    return true;
}

bool XPlaneClient_init(const char *ip, unsigned short port){
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed.\n");
        return false;
    }

    winsock_started = true;

    udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (udp_socket == INVALID_SOCKET) {
        printf("UDP socket creation failed: %d\n", WSAGetLastError());
        XPlaneClient_close();
        return false;
    }

    struct sockaddr_in local_address = {0};

    local_address.sin_family = AF_INET;
    local_address.sin_port = htons(LOCAL_UDP_PORT);
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(udp_socket, (struct sockaddr *)&local_address, sizeof(local_address)) == SOCKET_ERROR){
        printf("UDP bind failed: %d\n", WSAGetLastError());
        XPlaneClient_close();
        return false;
    }

    memset(&xplane_address, 0, sizeof(xplane_address));
    xplane_address.sin_family = AF_INET;
    xplane_address.sin_port = htons(port);
    xplane_address.sin_addr.s_addr = inet_addr(ip);

    if (xplane_address.sin_addr.s_addr == INADDR_NONE) {
        printf("Invalid X-Plane IP address.\n");
        XPlaneClient_close();
        return false;
    }

    u_long nonblocking = 1;

    if (ioctlsocket(udp_socket, FIONBIO, &nonblocking)
        == SOCKET_ERROR) {

        printf("Failed to configure nonblocking UDP socket: %d\n", WSAGetLastError());

        XPlaneClient_close();
        return false;
    }

    if (!XPlaneClient_init_subscriptions()) {
        XPlaneClient_close();
        return false;
    }

    return true;
}


bool XPlaneClient_send_command(const char *command){
    if (udp_socket == INVALID_SOCKET || command == NULL) {
        return false;
    }

    char packet[CMND_PACKET_SIZE];
    size_t command_length = strlen(command);

    if (command_length > sizeof(packet) - 6) {
        return false;
    }

    memcpy(packet, "CMND", 4);
    packet[4] = '\0';
    memcpy(packet + 5, command, command_length + 1);

    return send_packet(packet, (int)(6 + command_length));
}


static void process_rref_packet(const char *packet, int received){
    if (received < 13) {
        return;
    }

    for (int offset = 5; offset + 8 <= received; offset += 8) {

        int32_t index;
        float value;

        memcpy(&index, packet + offset, sizeof(index));
        memcpy(&value, packet + offset + 4, sizeof(value));
        
        // BEGINING OF DIFERENTIAL PROCESSING

        if (index == RREFID_HEADING && value >= 0.0f && value <= 360.0f) {
            current_heading = value;
            heading_valid = true;

            if (heading_pending && fabsf(heading_difference(value, pending_heading)) <= HEADING_TOLERANCE) {
                heading_pending = false;
            }
        }
    }
    return;
}


void XPlaneClient_poll(void){
    if (udp_socket == INVALID_SOCKET) {
        return;
    }

    char packet[UDP_BUFFER_SIZE];

    for (;;) {
        struct sockaddr_in sender;
        int sender_length = sizeof(sender);

        int received = recvfrom(
            udp_socket,
            packet,
            sizeof(packet),
            0,
            (struct sockaddr *)&sender,
            &sender_length
        );

        if (received == SOCKET_ERROR) {
            int error = WSAGetLastError();

            if (error != WSAEWOULDBLOCK) {
                printf("UDP receive error: %d\n", error);
            }

            break;
        }

        if (received >= 4 && memcmp(packet, "RREF", 4) == 0) {
            process_rref_packet(packet, received);
        }
    }

    DWORD now = GetTickCount();

    if (heading_pending && now - pending_since >= HEADING_CONFIRM_TIMEOUT) {
        heading_pending = false;
    }
}


bool XPlaneClient_adjust_heading(int direction){
    if (direction == 0) {
        return true;
    }

    if (!heading_valid) {
        return false;
    }

    float base_heading = heading_pending
                       ? pending_heading
                       : current_heading;

    float new_heading = normalize_heading(
        base_heading + (float)direction
    );

    if (!write_heading(new_heading)) {
        return false;
    }

    pending_heading = new_heading;
    pending_since = GetTickCount();
    heading_pending = true;

    return true;
}


bool XPlaneClient_adjust_com1(int direction){
    if (direction == 0) {
        return true;
    }

    return XPlaneClient_send_command(
        direction > 0
            ? "sim/radios/stby_com1_coarse_up"
            : "sim/radios/stby_com1_coarse_down"
    );
}


void XPlaneClient_close(void){
    if (udp_socket != INVALID_SOCKET) {
        closesocket(udp_socket);
        udp_socket = INVALID_SOCKET;
    }

    if (winsock_started) {
        WSACleanup();
        winsock_started = false;
    }

    heading_valid = false;
    heading_pending = false;
}