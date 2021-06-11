# Developped by : Axel Marceau
#        GitHub : https://github.com/Reapper
#          Mail : axel@e-marceau.com

quitFlag = 0
while quitFlag != 1:
    binary = input("Binary ? <type \"quit\" to quit the decoder> \n")
    if (binary != "quit"):
        i = 1
        j = 21

        print(binary[0:16], binary[16:20], binary[21])
        print("BIN  P HEX NIBLE    N° BIT")

        while i < 14:
            hex1 = int(binary[j])*1
            hex2 = int(binary[j+1])*2
            hex3 = int(binary[j+2])*4
            hex4 = int(binary[j+3])*8
            resHex = hex1 + hex2 + hex3 + hex4
            print(binary[j:j+4],binary[j+4], hex(resHex), i,"       ", j)
            i+=1
            j+=5
            

        print(binary[j+5:j+9], binary[j+10], binary[j+11:j+15], binary[j+16], binary[j+17:])
        print("bitlength:", len(binary))
        binary = ""
    else :
        quitFlag = 1
print("END")
