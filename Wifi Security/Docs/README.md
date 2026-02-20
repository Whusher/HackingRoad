# Network Commands Reference

A practical reference guide for wireless network interface management, traffic routing, and access point configuration on Linux systems.

---

## Table of Contents

- [Interface Management](#interface-management)
- [Virtual Interfaces](#virtual-interfaces)
- [MAC Address Management](#mac-address-management)
- [Network Scanning](#network-scanning)
- [IP Address Configuration](#ip-address-configuration)
- [IP Tables & Traffic Routing](#ip-tables--traffic-routing)
- [Access Point Setup](#access-point-setup)
- [Restore Network Services](#restore-network-services)

---

## Interface Management

### List all network interfaces
```bash
ifconfig
```

### List wireless interfaces
```bash
iwconfig
```

### Bring an interface down
Useful before running `macchanger` scripts or modifying kernel modules for chipset drivers.
```bash
ifconfig wlan0 down
```

### Bring an interface up
```bash
ifconfig wlan0mon up
```

### Check and kill conflicting processes
Before enabling monitor mode, services like `wpa_supplicant` and NetworkManager may interfere. Use the following to detect and stop them:
```bash
airmon-ng check kill
```

---

## Virtual Interfaces

### Create a monitor mode interface
```bash
iw dev wlan0 interface add wlan0mon type monitor
```

### Create a managed AP interface
```bash
iw dev wlan0 interface add wlan0ap type __ap
```

---

## MAC Address Management

### List OUI addresses and filter by vendor
```bash
macchanger -l | grep SECURITY
```

### Assign a specific MAC address to an interface
```bash
macchanger --mac 00:e0:5a:aa:aa:aa wlan0mon
```

> **Note:** Ensure the interface is brought down with `ifconfig wlan0 down` before changing the MAC address.

---

## Network Scanning

### Scan for nearby wireless networks
```bash
airodump-ng wlan0mon
```

---

## IP Address Configuration

### Assign a static IP to an interface
```bash
ip addr flush dev wlan0ap
ip addr add 192.168.1.1/24 dev wlan0ap
```

---

## IP Tables & Traffic Routing

The following rules configure NAT, IP forwarding, and HTTP traffic redirection through the AP interface.

### Inspect current NAT PREROUTING rules
```bash
iptables -t nat -L PREROUTING -n -v
```

### Redirect HTTP traffic to the local AP host
```bash
iptables -t nat -A PREROUTING -i wlan0ap -p tcp --dport 80 -j DNAT --to-destination 192.168.1.1:80
```

### Enable IP forwarding
```bash
echo 1 > /proc/sys/net/ipv4/ip_forward
```

### Enable NAT masquerading on the upstream interface
```bash
iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
```

### Allow forwarding between AP and upstream interfaces
```bash
iptables -A FORWARD -i wlan0ap -o eth0 -j ACCEPT
iptables -A FORWARD -i eth0 -o wlan0ap -m state --state RELATED,ESTABLISHED -j ACCEPT
```

### Bypass redirection for major providers (allowlist)
Allow HTTP traffic to specific IP ranges to pass through without being redirected:
```bash
# Google
iptables -t nat -A PREROUTING -i wlan0ap -p tcp --dport 80 -d 142.250.0.0/15 -j ACCEPT

# Apple
iptables -t nat -A PREROUTING -i wlan0ap -p tcp --dport 80 -d 17.0.0.0/8 -j ACCEPT

# Microsoft
iptables -t nat -A PREROUTING -i wlan0ap -p tcp --dport 80 -d 13.107.0.0/16 -j ACCEPT
```

> **Note:** Allowlist rules should be placed **before** the catch-all redirect rule in the PREROUTING chain so that they are evaluated first.

---

## Access Point Setup

### Start the access point using `hostapd`
```bash
hostapd -dd /path/to/hostapd.conf
```

### Start DNS resolution for the AP using `dnsmasq`
```bash
dnsmasq -C /path/to/dnsmasq-ap.conf --no-daemon
```

> Replace `/path/to/` with the actual path to your configuration files.

---

## Restore Network Services

### Restart NetworkManager and `wpa_supplicant`
Run this after finishing your session to restore normal network connectivity:
```bash
systemctl restart NetworkManager
```

---

## Prerequisites

The following tools must be installed on your system:

- `aircrack-ng` suite (`airmon-ng`, `airodump-ng`)
- `macchanger`
- `hostapd` / `hostapd-wpe`
- `dnsmasq`
- `iptables`
- `iw` / `iwconfig` (`wireless-tools`)

---

## ⚠️ Disclaimer

This guide is intended for **authorized security testing and educational purposes only**. Unauthorized interception or manipulation of network traffic is illegal. Always obtain proper written permission before performing any wireless security assessments.