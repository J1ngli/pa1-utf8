#include <stdio.h>
#include <stdint.h>

int32_t is_ascii(char str[]){
    int i =0;
    while(str[i]!='\0'){
        if((unsigned char)str[i]>127){
            return 0;
        }
        i++;
    }
    return 1;
}

int32_t capitalize_ascii(char str[]){
    int i=0;
    int n_changes=0;
    while(str[i]!='\0'){
        if(str[i]<='z'&&str[i]>='a'){
            str[i]-=32;
            n_changes++;
        }
        i++;
    }
    return n_changes;
}

int32_t width_from_start_byte(char start_byte){
    if((start_byte & 0b10000000)== 0b00000000){
        return 1;

    }
    if((start_byte & 0b11100000)==0b11000000){
        return 2;
    }
    if((start_byte & 0b11110000)==0b11100000){
        return 3;
    }
    if((start_byte & 0b11111000)==0b11110000){
        return 4;
    }
    return -1;
    
}


int32_t utf8_strlen(char str[]) {
    int i = 0;
    int length = 0;

    while (str[i] != '\0') {
        unsigned char byte = str[i];

        if ((byte & 0b10000000) == 0b00000000) {
        // byte
            i += 1;
        } 
        else if ((byte & 0b11100000) == 0b11000000) {
            //2 byte
            if ((str[i + 1] & 0b11000000) != 0b10000000) return -1; 
            i += 2;
        } 
        else if ((byte & 0b11110000) == 0b11100000) {
            // 3 byte
            if ((str[i + 1] & 0b11000000) != 0b10000000 || (str[i + 2] & 0b11000000) != 0b10000000) return -1;
            i += 3;
        } 
        else if ((byte & 0b11111000) == 0b11110000) {
            // 4 byte
            if ((str[i + 1] & 0b11000000) != 0b10000000 || (str[i + 2] & 0b11000000) != 0b10000000 || (str[i + 3] & 0b11000000) != 0b10000000) return -1;
            i += 4;
        } 
        else {
            // Invalid 
            return -1;
        }

        // Increase codepoint length for each valid sequence
        length++;
    }

    return length;
}

int main(){
    printf("Is 🔥 ASCII? %d\n", is_ascii("🔥"));
    printf("Is abcd ASCII? %d\n", is_ascii("abcd"));
    
    int32_t ret = 0;
    char str[] = "abcd";
    ret = capitalize_ascii(str);
    printf("Capitalized String: %s\nCharacters updated: %d\n", str, ret);
    char s[] = "Héy"; // same as { 'H', 0xC3, 0xA9, 'y', 0 },   é is start byte + 1 cont. byte
    printf("Width: %d bytes\n", width_from_start_byte(s[1])); // start byte 0xC3 indicates 2-byte sequence
    printf("Width: %d bytes\n", width_from_start_byte(s[2])); // start byte 0xA9 is a continuation byte, not a start byte
    char J[] = "Joséph";  // 6 codepoints, 7 bytes (including 'é' as 2 bytes)
    printf("Length of string %s is %d\n", J, utf8_strlen(J));  

    int32_t codepoint_index_to_byte_index(char str[], int32_t cpi){

    }
    
    return 0;
}