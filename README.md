# ARP Poisoning
After execution, the program will wait for the target to make an ARP request 
and instantly respond with the malicious ARP response.\
Use flag -r to repeate the response every second, -v for a verbose mode and -f to not wait for the arp request from target to send the arp response. (42 Project)

## Authors

- [@shad0cks](https://www.github.com/shad0cks)


## Usage

```
sysctl -w net.ipv4.ip_forward=1 &&
  make && ./ft_malcolm <OPTIONS> <SRC_IP> <SRC_MAC> <TARGET_IP> <TARGET_MAC>
```

| Parameter | Exemple     | Description                |
| :-------- | :------- | :------------------------- |
| `source ip` | `10.2.0.1` | **Required**. Gateway IP |
| `source mac address` | `aa:aa:aa:aa:aa:aa` | **Required**. Attacker MAC |
| `target ip` | `10.2.0.12` | **Required**. Victim IP |
| `target mac address` | `bb:bb:bb:bb:bb:bb` | **Required**. Victim MAC |



## Flags

- -r : Repeate mode, will send ARP REPONSE every second
- -v : Verbose mode, will print more information about process
- -f:  Force mode, dont wait for the arp request from target to send the malicious arp response.
