#!/usr/bin/env python3

"""
little-endian means storing a multi-byte number with the least significant byte first.
Example: 
Big-endian bytes: 12 34 56 78
Little-endian bytes: 78 56 34 12

"""
import struct, sys
##payload = b"GRGRVISHKARSFDJDJFJFDJFJDJFJDJJDFJDNFJNJFDNJFNDJABCDEFGHIJKLMNOPGG" ##66 characters
##payload = b"GRGRVISHKARSFDJDJFJFDJFJDJFJDJJDFJDNFJNJFDNJFNDJABCDEFGHIJKLMNOPGGSFJDJFJDFDF" ##77 characters
payload = b"GRGRVISHKARSFDJDJFJFDJFJDJFJDJJDFJDNFJNJFDNJFNDJABCDEFGHIJKLMNOPGGSFJDJFJDF" ##75 characters
##payload = b"GRGRVISHKARSFDJDJFJFDJFJDJFJDJJDFJDNFJNJFDNJFNDJABCDEFGHIJKLMNOPGsdjfG" ##70 characters
## little digression here
##for i in range(0, 100):
##    letter = random.choice(string.ascii_uppercase)   # A-Z
##    payload = b"A" + payload + letter.encode()  # Prepend 'A' and append a random uppercase letter
#payload = b"GRGRVISHKARSFDJDJFJFDJFJDJFJDJJDFJDNFJNJFDNJFNDJDJFDJFJDJFJDJDFJDFJDHFDJFHJDHFJHDFJDHFJDHFJDHFJDHFJDFHJDFJDHFJDHFJHDFJHFJHIERUIPEURIJOIEJFIOEFHOEFHIOERIOERIOEURIOEUROIREUOIRUOIREUOIURE"
blob = struct.pack ("<I", len(payload)) + payload
sys.stdout.buffer.write(blob)