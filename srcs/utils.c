#include "../include.h"

int dns_lookup(char * addr)
{
	struct addrinfo * res;
	struct addrinfo * temp;
	struct addrinfo hints;

	ft_memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;

	int err =  getaddrinfo(addr, NULL, &hints, &res);

	if (err == 0)
	{
		freeaddrinfo(res);
		return (0);
	}
	else if (err == EAI_NONAME)
		dprintf(2, "IP Error : Name or service not know for : %s\n", addr);
	else if (err == -5)
		dprintf(2, "IP Error : ft_malcolm: %s: No address associated with hostname!\n", addr);
	else
		dprintf(2, "IP Error : Failed to find %s in the network\n", addr);
	
	freeaddrinfo(res);
	return (-1);
}

int check_mac_format(char *mac)
{
	int i = 0;
	if (ft_strlen(mac) != 17)
		return (-1);
	
	while (i < 17)
	{
		if (i % 3 == 2)
		{
			if (mac[i] != ':')
				return (-1);
		}
		else if (i % 3 == 1 || i % 3 == 0)
		{
			if (!(mac[i] >= '0' && mac[i] <= '9') && !(mac[i] >= 'a' && mac[i] <= 'f') && !(mac[i] >= 'A' && mac[i] <= 'F'))
				return (-1);
		}
		i++;
	}
	return (0);
}

unsigned char hex_digit( char ch )
{
    if(('0' <= ch) && (ch <= '9'))
        ch -= '0';
    else
    {
        if(('a' <= ch) && (ch <= 'f'))
            ch += 10 - 'a';
        else
        {
            if(('A' <= ch) && (ch <= 'F'))
                ch += 10 - 'A';
            else
                ch = 16;
        }
    }
    return ch;
}

void convert_mac(const char * mac_addr,  unsigned char * dest)
{
    unsigned char mac[6];

    for (uint idx = 0; idx < sizeof(mac) / sizeof(mac[0]); ++idx)
    {
        mac[idx] = hex_digit(mac_addr[3 * idx]) << 4;
        mac[idx] |= hex_digit(mac_addr[1 + 3 * idx]);
    }
    ft_memcpy(dest, mac, MAC_LENGTH);
}

void	*ft_memset(void *pointer, int value, size_t count)
{
	unsigned int	index;
	unsigned char	*result;

	result = (unsigned char *)pointer;
	index = 0;
	while (index < count)
	{
		result[index] = value;
		index++;
	}
	return ((void *)result);
}

int	ft_memcmp(const void *str1, const void *str2, size_t n)
{
	const unsigned char	*cast_str1;
	const unsigned char	*cast_str2;

	cast_str1 = (const unsigned char *)str1;
	cast_str2 = (const unsigned char *)str2;
	if (str1 == str2 || n == 0)
		return (0);
	while (n > 0)
	{
		if (*cast_str1 != *cast_str2)
			return ((*cast_str1) - (*cast_str2));
		cast_str1++;
		cast_str2++;
		n--;
	}
	return (0);
}

void	*ft_memcpy( void *destination, const void *source, size_t size )
{
	const unsigned char	*cast_src;
	unsigned char		*cast_dest;
	int					i;

	cast_src = (const unsigned char *) source;
	cast_dest = (unsigned char *) destination;
	i = 0;
	if ((size == 0) || (destination == source))
		return (destination);
	while ((size_t)i < size)
	{
		cast_dest[i] = cast_src[i];
		i++;
	}
	return ((void *)cast_dest);
}

size_t	ft_strlen(const char *str)
{
	size_t	index;

	index = 0;
	while (str[index] != '\0')
		index++;
	return (index);
}