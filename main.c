int main() {
    char input[1000];
    printf("Enter a UTF-8 encoded string: ");
    fgets(input, sizeof(input), stdin);

    // Remove the newline character from input
    int len = 0;
    while (input[len] != '\0') len++;
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    // Check if the string is valid ASCII
    int ascii_valid = is_ascii(input);
    printf("Valid ASCII: %s\n", ascii_valid ? "true" : "false");

    // Uppercase ASCII
    char uppercase_input[1000];
    for (int i = 0; i < len; i++) uppercase_input[i] = input[i];
    capitalize_ascii(uppercase_input);
    printf("Uppercased ASCII: \"%s\"\n", uppercase_input);

    // Length in bytes
    printf("Length in bytes: %d\n", len);

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
    int byte_index = 0;
    for (int j = 0; j < codepoints; j++) {
        if (is_animal_emoji_at(input, j)) {  // Corrected: pass input and index j
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
