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
    if((start_byte & 0b11110000)==0b11110000){
        return 4;
    }
    return -1;
    
}

int32_t utf8_strlen(char str[]){
    int i=0;
    int length=0;
    while(str[i]!='\0'){
        if((str[i] & 0b10000000)== 0b00000000){
            length++;
        }
        if((str[i] & 0b11100000)==0b11000000){
            length++;
        }
        if((str[i] & 0b11110000)==0b11100000){
            length++;
        }
        if((str[i] & 0b11110000)==0b11110000){
            length++;
        }
        i++;
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
    char J[] = "Joséph";
    printf("Length of string %s is %d\n", J, utf8_strlen(str));  // 6 codepoints, (even though 7 bytes)

    return 0;
}