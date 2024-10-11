#include <stdio.h>
#include <stdint.h>

// Function to check if the input string is valid UTF-8
int is_valid_utf8(const char *str) {
    const unsigned char *bytes = (const unsigned char *)str;
    int i = 0;
    while (bytes[i]) {
        if ((bytes[i] & 0x80) == 0) {
            // 1-byte character (ASCII)
            i++;
        } else if ((bytes[i] & 0xE0) == 0xC0) {
            // 2-byte character
            if ((bytes[i + 1] & 0xC0) != 0x80) return 0;
            i += 2;
        } else if ((bytes[i] & 0xF0) == 0xE0) {
            // 3-byte character
            if ((bytes[i + 1] & 0xC0) != 0x80 || (bytes[i + 2] & 0xC0) != 0x80) return 0;
            i += 3;
        } else if ((bytes[i] & 0xF8) == 0xF0) {
            // 4-byte character
            if ((bytes[i + 1] & 0xC0) != 0x80 || (bytes[i + 2] & 0xC0) != 0x80 || (bytes[i + 3] & 0xC0) != 0x80) return 0;
            i += 4;
        } else {
            return 0; // Invalid start byte
        }
    }
    return 1; // Valid UTF-8
}

// Existing functions...

int main() {
    char input[1000];
    printf("Enter a UTF-8 encoded string: ");
    fgets(input, sizeof(input), stdin);

    // Trim newline character if present
    int len = 0;
    while (input[len] != '\0') len++;
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    // Validate UTF-8 input
    if (!is_valid_utf8(input)) {
        fprintf(stderr, "Error: Invalid UTF-8 encoding\n");
        return 1; // Exit with an error code
    }

    // Proceed with the rest of your program
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