from scapy.all import *
from scapy.layers.inet import IP, UDP, TCP, ICMP, GRE
import time

def send_udp_packet():
    packet = IP(dst="127.0.0.1") / UDP(sport=12345, dport=80) / Raw(load="UDP test packet")
    send(packet, verbose=False)

def send_tcp_packet():
    packet = IP(dst="127.0.0.1") / TCP(sport=12345, dport=80, flags="S") / Raw(load="TCP test packet")
    send(packet, verbose=False)

def send_icmp_packet():
    packet = IP(dst="127.0.0.1") / ICMP(type="echo-request") / Raw(load="ICMP test packet")
    send(packet, verbose=False)

def send_esp_packet():
    packet = IP(dst="127.0.0.1", proto=50) / Raw(load="ESP test packet")
    send(packet, verbose=False)

def send_gre_packet():
    packet = IP(dst="127.0.0.1") / GRE() / Raw(load="GRE test packet")
    send(packet, verbose=False)

def continuous_packet_sending(interval=1):
    try:
        while True:
            print("Sending UDP packet...")
            send_udp_packet()
            
            print("Sending TCP packet...")
            send_tcp_packet()
            
            print("Sending ICMP packet...")
            send_icmp_packet()
            
            print("Sending ESP packet...")
            send_esp_packet()
            
            print("Sending GRE packet...")
            send_gre_packet()
            
            print("Packets sent. Waiting for the next round...\n")
            time.sleep(interval)
    
    except KeyboardInterrupt:
        print("Packet sending interrupted by user.")

if __name__ == "__main__":
    continuous_packet_sending(interval=1)
