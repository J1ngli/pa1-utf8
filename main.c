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

int32_t codepoint_index_to_byte_index(char str[], int32_t cpi){
    int i =0;
    int byte_idx=0;
    while(i!=cpi){
        unsigned char byte = str[i];

        if((byte & 0b10000000) == 0b00000000){
            byte_idx++;
        }
        else if ((byte & 0b11100000)==0b11000000){
            byte_idx+=2;
        }
        else if((byte & 0b11110000)== 0b11100000){
            byte_idx+=3;
        }
        else if((byte & 0b11111000)== 0b11110000){
            byte_idx+=4;
        }
        else {
            return -1;
        }
        i++;
    }
    return byte_idx;
}

void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[]) {
    // Validate inputs
    if (cpi_start < 0 || cpi_end < 0 || cpi_start > cpi_end) {
        return; // No effect if invalid
    }

    int byte_index = 0; // Current byte index
    int cp_index = 0; // Current codepoint index
    int start_byte_index = -1; // Start byte index for the substring
    int end_byte_index = -1; // End byte index for the substring

    // Traverse the string to find the byte indices for cpi_start and cpi_end
    while (str[byte_index] != '\0') {
        // Check if we've reached the start codepoint index
        if (cp_index == cpi_start) {
            start_byte_index = byte_index; // Store start byte index
        }
        // Check if we've reached the end codepoint index
        if (cp_index == cpi_end) {
            end_byte_index = byte_index; // Store end byte index
            break; // No need to continue if we found the end
        }

        // Determine the width of the current UTF-8 character
        int width = width_from_start_byte(str[byte_index]);
        if (width == -1) {
            return; // Invalid UTF-8 encoding, exit
        }

        // Move the byte index forward by the width of the character
        byte_index += width;
        cp_index++; // Increment codepoint index
    }

    // If the end byte index wasn't found, it means cpi_end is out of bounds
    if (end_byte_index == -1) {
        end_byte_index = byte_index; // Set it to the end of the string
    }

    // Copy the substring from start to end byte index
    int length = end_byte_index - start_byte_index; // Length of substring
    for (int j = 0; j < length; j++) {
        result[j] = str[start_byte_index + j]; // Copy bytes
    }
    result[length] = '\0'; // Null-terminate the result
}
//Milestone3
int32_t codepoint_at(char str[], int32_t cpi){

}

char is_animal_emoji_at(char str[], int32_t cpi){
    
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
    //Milestone 2 bite indx
    char str2[] = "Joséph";
    int32_t idx = 4;
    printf("Codepoint index %d is byte index %d\n", idx, codepoint_index_to_byte_index("Joséph", idx));
    //Substring Milestone2
    char result[17];
    utf8_substring("🦀🦮🦮🦀🦀🦮🦮", 3, 7, result);
    printf("String: 🦀🦮🦮🦀🦀🦮🦮\nSubstring: %s\n", result); // these emoji are 4 bytes long

    //Milestone3
    

    return 0;
}


