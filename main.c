#include <stdio.h>
#include <stdint.h>

int32_t is_ascii(char str[]) {
    int i = 0;
    while (str[i] != '\0') {
        if ((unsigned char)str[i] > 127) {
            return 0;
        }
        i++;
    }
    return 1;
}

int32_t capitalize_ascii(char str[]) {
    int i = 0;
    int n_changes = 0;
    while (str[i] != '\0') {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= 32;  // Convert to uppercase
            n_changes++;
        }
        i++;
    }
    return n_changes;
}

int32_t width_from_start_byte(char start_byte) {
    if ((start_byte & 0b10000000) == 0b00000000) {
        return 1;
    }
    if ((start_byte & 0b11100000) == 0b11000000) {
        return 2;
    }
    if ((start_byte & 0b11110000) == 0b11100000) {
        return 3;
    }
    if ((start_byte & 0b11111000) == 0b11110000) {
        return 4;
    }
    return -1;
}

int32_t utf8_strlen(char str[]) {
    int i = 0;
    int length = 0;

    while (str[i] != '\0') {
        unsigned char byte = str[i];
        int width = width_from_start_byte(byte);
        if (width == -1) {
            return -1; // Invalid UTF-8 sequence
        }
        i += width; // Move to next character
        length++;   // Count valid code points
    }

    return length;
}

int32_t codepoint_at(char str[], int32_t cpi) {
    if (cpi < 0) return -1;  // Invalid codepoint index

    int byte_index = 0;  // Current byte index in the string
    int cp_index = 0;    // Current codepoint index

    while (str[byte_index] != '\0') {
        int width = width_from_start_byte(str[byte_index]);
        if (width == -1) return -1;  // Invalid UTF-8 start byte

        if (cp_index == cpi) {
            int32_t codepoint = 0;
            if (width == 1) {
                codepoint = str[byte_index];
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
            return codepoint;
        }

        byte_index += width;
        cp_index++;
    }

    return -1;
}

int32_t is_animal_emoji_at(char str[], int32_t cpi) {
    int32_t codepoint = codepoint_at(str, cpi);
    if (codepoint == -1) return 0;

    // Unicode ranges for animal emojis
    if ((codepoint >= 0x1F400 && codepoint <= 0x1F43F) || // 🐀 to 🐿️
        (codepoint >= 0x1F980 && codepoint <= 0x1F9AC)) { // 🦀 to 🦮
        return 1;
    }

    return 0;
}

void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[]) {
    if (cpi_start < 0 || cpi_end < 0 || cpi_start > cpi_end) return;

    int byte_index = 0, cp_index = 0, start_byte_index = -1, end_byte_index = -1;

    while (str[byte_index] != '\0') {
        if (cp_index == cpi_start) start_byte_index = byte_index;
        if (cp_index == cpi_end) {
            end_byte_index = byte_index;
            break;
        }

        int width = width_from_start_byte(str[byte_index]);
        if (width == -1) return;

        byte_index += width;
        cp_index++;
    }

    if (end_byte_index == -1) end_byte_index = byte_index;

    int length = end_byte_index - start_byte_index;
    for (int j = 0; j < length; j++) {
        result[j] = str[start_byte_index + j];
    }
    result[length] = '\0';
}

int main() {
    char input[1000];
    printf("Enter a UTF-8 encoded string: ");
    fgets(input, sizeof(input), stdin);

    int len = 0;
    while (input[len] != '\0') len++;
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    printf("Valid ASCII: %s\n", is_ascii(input) ? "true" : "false");

    char uppercase_input[1000];
    for (int i = 0; i < len; i++) uppercase_input[i] = input[i];
    capitalize_ascii(uppercase_input);
    printf("Uppercased ASCII: \"%s\"\n", uppercase_input);

    printf("Length in bytes: %d\n", len);

    int codepoints = utf8_strlen(input);
    printf("Number of code points: %d\n", codepoints);

    printf("Bytes per code point: ");
    int i = 0;
    while (input[i] != '\0') {
        int width = width_from_start_byte(input[i]);
        printf("%d ", width);
        i += width;
    }
    printf("\n");

    char result[100];
    utf8_substring(input, 0, 6, result);
    printf("Substring of the first 6 code points: \"%s\"\n", result);

    printf("Code points as decimal numbers: ");
    for (int j = 0; j < codepoints; j++) {
        printf("%d ", codepoint_at(input, j));
    }
    printf("\n");

    printf("Animal emojis: ");
    int byte_index = 0;
    for (int j = 0; j < codepoints; j++) {
        if (is_animal_emoji_at(input, j)) {
            int width = width_from_start_byte(input[byte_index]);
            for (int k = 0; k < width; k++) {
                printf("%c", input[byte_index + k]);
            }
            printf(" ");
        }
        byte_index += width_from_start_byte(input[byte_index]);
    }
    printf("\n");

    return 0;
}