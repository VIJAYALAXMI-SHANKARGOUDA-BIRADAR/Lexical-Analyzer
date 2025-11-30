#include<stdio.h>


char *my_strcpy(char* dest,char*src)
{
    int i=0;

    while(src[i] != '\0')
    {
        dest[i]=src[i];
        i++;

    }
    dest[i]='\0';

    return dest;
}

/*void my_strcpy(char* dest,char*src)
{
    
    while(*src!='\0')
    {
        *dest=*src;

        src++;
        dest++;
    }
    *dest='\0';

    

}*/

int main()
{
    char str1[100], str2[100];

    printf("Enter a string\n");
    scanf("%s",str1);

    my_strcpy(str2,str1);

    printf("after copying \nstr1=%s\nstr2=%s\n",str1,str2);
    return 0;



    int n_dec = 123;        // Decimal
    int n_oct = 0123;       // Octal
    int n_hex = 0x00;       // Hexadecimal
    int n_bin = 0b101;      // Binary


    int invalid_oct = 012;   // Error: invalid octal digit upto 7   3)invalid octa
    int invalid_hex = 0x1h;     // Error: invalid hex digit      4)invalid hexa
    int incomplete_hex = 0x1    // Error: incomplete hex         invalid hexa
    int n_bin = 0b102;  
    int trailing_char = 123a;  // Error: identifier after number    

    char invalid_char = 'AB'; // Error: too many character


}

