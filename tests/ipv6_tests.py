from scapy.all import *
from scapy.layers.inet import UDP, TCP
from scapy.layers.inet6 import IPv6, ICMPv6EchoRequest
import time

def send_udp_ipv6_packet():
    packet = IPv6(dst="::1") / UDP(sport=12345, dport=80) / Raw(load="UDP IPv6 test packet")
    send(packet, verbose=False)

def send_tcp_ipv6_packet():
    packet = IPv6(dst="::1") / TCP(sport=12345, dport=80, flags="S") / Raw(load="TCP IPv6 test packet")
    send(packet, verbose=False)

def send_icmpv6_packet():
    packet = IPv6(dst="::1") / ICMPv6EchoRequest(data="ICMPv6 test packet")
    send(packet, verbose=False)

def send_esp_ipv6_packet():
    esp_header = b'\x12\x34\x00\x01'
    esp_payload = b'ESP IPv6 test packet'
    packet = IPv6(dst="::1") / Raw(load=esp_header + esp_payload)
    send(packet, verbose=False)

def continuous_ipv6_packet_sending(interval=1):
    try:
        while True:
            print("Sending UDP IPv6 packet...")
            send_udp_ipv6_packet()
            
            print("Sending TCP IPv6 packet...")
            send_tcp_ipv6_packet()
            
            print("Sending ICMPv6 packet...")
            send_icmpv6_packet()
            
            print("Sending ESP IPv6 packet...")
            send_esp_ipv6_packet()
            
            print("IPv6 packets sent. Waiting for the next round...\n")
            time.sleep(interval)
    
    except KeyboardInterrupt:
        print("Packet sending interrupted by user.")

if __name__ == "__main__":
    continuous_ipv6_packet_sending(interval=1)
