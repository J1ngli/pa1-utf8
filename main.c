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
    
}
int main(){
    printf("Is 🔥 ASCII? %d\n", is_ascii("🔥"));
    printf("Is abcd ASCII? %d\n", is_ascii("abcd"));
    
    int32_t ret = 0;
    char str[] = "abcd";
    ret = capitalize_ascii(str);
    printf("Capitalized String: %s\nCharacters updated: %d\n", str, ret);

    return 0;
}