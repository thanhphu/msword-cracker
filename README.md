# msword-cracker

Crack Microsoft Word 95 and 97 passwords with the magic of wvware.

You will need:
    libwv-dev libgsf-1-dev glib2-dev 

This software is licensed under the GNU GPL v3 or any later version.

This version is updated to compile on modern Linux

## How to use

```
sudo apt install libwv-dev libgsf-1-dev libglib2.0-dev 
make
./msword-cracker { -p password | -w wordlist | -c charset } file.doc
```

## Alternative 1: msoffcrypto-crack.py

`python3 msoffcrypto-crack.py word-file.doc`

## Alternative 2: office2john + hashcat

`python office2john.py word-file.doc > hash.txt`

Edit hash.txt to look like
`$oldoffice$1*xxx*xxx*xxx`

Download a good password list
```
$ curl -LO https://github.com/danielmiessler/SecLists/raw/601038eb4ea18c97177b43a757286d3c8a815db8/Passwords/merged.txt.tar.gz
tar xf merged.txt.tar.gz
```

Use hashcat
`hashcat -a 0 -m 9700 --status --show hash.txt merged.txt`