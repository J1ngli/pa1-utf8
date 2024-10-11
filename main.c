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
        return; 
    }

    int byte_index = 0; // Current byte index
    int cp_index = 0; // Current codepoint index
    int start_byte_index = -1; // Start byte index for the substring
    int end_byte_index = -1; // End byte index for the substring

    
    while (str[byte_index] != '\0') {
        
        if (cp_index == cpi_start) {
            start_byte_index = byte_index; 
        }
       
        if (cp_index == cpi_end) {
            end_byte_index = byte_index; 
            break; 
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

    if (end_byte_index == -1) {
        end_byte_index = byte_index; 
    }


    int length = end_byte_index - start_byte_index; // Length of substring
    for (int j = 0; j < length; j++) {
        result[j] = str[start_byte_index + j]; // 
    }
    result[length] = '\0'; 
}
int32_t codepoint_at(char str[], int32_t cpi) {
    if (cpi < 0) return -1;  // Invalid codepoint index

    int byte_index = 0;  // Current byte index in the string
    int cp_index = 0;    // Current codepoint index

    while (str[byte_index] != '\0') {
        // Get the number of bytes in the current codepoint
        int width = width_from_start_byte(str[byte_index]);
        if (width == -1) return -1;  // Invalid UTF-8 start byte

        // Check if we've reached the desired codepoint index
        if (cp_index == cpi) {
            int32_t codepoint = 0;

            // Decode the codepoint based on its width
            if (width == 1) {
                codepoint = str[byte_index];  // Single-byte (ASCII)
            } else if (width == 2) {
                codepoint = ((str[byte_index] & 0x1F) << 6) | (str[byte_index + 1] & 0x3F);
            } else if (width == 3) {
                codepoint = ((str[byte_index] & 0x0F) << 12) |
                            ((str[byte_index + 1] & 0x3F) << 6) |
                            (str[byte_index + 2] & 0x3F);
            } else if (width == 4) {
                codepoint = ((str[byte_index] & 0x07) << 18) |
                            ((str[byte_index + 1] & 0x3F) << 12) |
                            ((str[byte_index + 2] & 0x3F) << 6) |
                            (str[byte_index + 3] & 0x3F);
            }

            return codepoint;  // Return the decoded codepoint
        }

        // Move to the next codepoint by skipping the correct number of bytes
        byte_index += width;  // Update byte index by the width of the current codepoint
        cp_index++;  // Increment codepoint index
    }

    return -1;  // Codepoint index out of bounds
}


char is_animal_emoji_at(char str[], int32_t cpi) {
    int32_t codepoint = codepoint_at(str, cpi);
    if (codepoint == -1) return 0; // Invalid codepoint or index

    // Check if the codepoint is in the animal emoji ranges
    if ((codepoint >= 0x1F400 && codepoint <= 0x1F43F) || // Range: 🐀 to 🐿️
        (codepoint >= 0x1F980 && codepoint <= 0x1F9AC)) { // Range: 🦀 to 🦮
        return 1; // True: It is an animal emoji
    }

    return 0; // False: It is not an animal emoji
}

int main(){
     char input[1000];
    printf("Enter a UTF-8 encoded string: ");
    fgets(input, sizeof(input), stdin);

    // Remove the newline character from input
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    // Check if the string is valid ASCII
    bool ascii_valid = is_ascii(input);
    printf("Valid ASCII: %s\n", ascii_valid ? "true" : "false");

    // Uppercase ASCII
    char uppercase_input[1000];
    strcpy(uppercase_input, input);
    capitalize_ascii(uppercase_input);
    printf("Uppercased ASCII: \"%s\"\n", uppercase_input);

    // Length in bytes
    printf("Length in bytes: %lu\n", strlen(input));

    // Number of code points
    int codepoints = utf8_strlen(input);
    printf("Number of code points: %d\n", codepoints);

    // Bytes per code point
    printf("Bytes per code point: ");
    int i = 0;
    while (input[i] != '\0') {
        int width = width_from_start_byte(input[i]);
        printf("%d ", width);
        i += width;
    }
    printf("\n");

    // Substring of the first 6 code points
    char result[100];
    utf8_substring(input, 0, 6, result);
    printf("Substring of the first 6 code points: \"%s\"\n", result);

    // Code points as decimal numbers
    printf("Code points as decimal numbers: ");
    for (int j = 0; j < codepoints; j++) {
        printf("%d ", codepoint_at(input, j));
    }
    printf("\n");

    // Animal emojis
    printf("Animal emojis: ");
    for (int j = 0; j < codepoints; j++) {
        int32_t codepoint = codepoint_at(input, j);
        if (is_animal_emoji(codepoint)) {
            printf("%c%c%c%c ", input[j*4], input[j*4 + 1], input[j*4 + 2], input[j*4 + 3]);  // print the emoji
        }
    }
    printf("\n");

    return 0;
}


