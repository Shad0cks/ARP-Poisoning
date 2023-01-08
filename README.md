# ARP Poisoning
After execution, the program will wait for the target to make an ARP request 
and instantly respond with the malicious ARP response.\
Use flag -r to repeate the response every second and -v for a verbose mode.

## Authors

- [@shad0cks](https://www.github.com/shad0cks)


## Usage

```
  make && ./ft_malcolm <OPTIONS> <SRC_IP> <SRC_MAC> <TARGET_IP> <TARGET_MAC>
```

| Parameter |  Description                |
| :-------- |  :------------------------- |
| `source ip` | **Required**. Gateway IP |
| `source mac address` | **Required**. Gateway MAC |
| `target ip` | **Required**. Victim IP |
| `target mac address` | **Required**. Victim MAC |



## Flags

- -r : Repeate mode, will send ARP REPONSE every second
- -v : Verbose mode, will print more information about process
