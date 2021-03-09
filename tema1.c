// Abagiu Ioan-Razvan 321CD

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h> //used for tolower()
#include <limits.h> //used for UINT_MAX

// function used in reverse() to swap chars
void swap(char *x, char *y) {
	char t = *x;
	*x = *y; 
	*y = t;
}

// function to reverse the buffer
char* reverse(char *buffer, int i, int j) {

	while (i < j)
		swap(&buffer[i++], &buffer[j--]);

	return buffer;
}

char* itoa(long value, char* buffer, int base) {

	// check for invalid base
	if (base < 2 || base > 32)
		return buffer;

	/* 
	check the sign of number but also
	keep the original number in value 
	*/

	long n = value;
	if(value < 0)
		n = -value;

	unsigned int i = 0;
	while (n)
	{
		int r = n % base;

		if (r >= 10)
			buffer[i++] = 65 + (r - 10);
		else
			buffer[i++] = 48 + r;

		n = n / base;
	}

	// check if number is 0
	if (i == 0)
		buffer[i++] = '0';

	/*
	if base is 10 and value is negative, the resulting string
	is preceded with a minus sign (-)
	With any other base, value is always considered unsigned
	*/
	if (value < 0 && base == 10)
		buffer[i++] = '-';

	buffer[i] = '\0'; // add null terminator

	// used tolower() function found in <ctype.h> for the hexa base
	if(base == 16) {

		for(i = 0; i < strlen(buffer); i++)
			buffer[i] = tolower(buffer[i]);
	}

	// reverse the string and return it
	return reverse(buffer, 0, i - 1);
}


static int write_stdout(const char *token, int length) {

	int rc;
	int bytes_written = 0;

	do {
		rc = write(1, token + bytes_written, length - bytes_written);
		if (rc < 0)
			return rc;

		bytes_written += rc;
	} while (bytes_written < length);

	return bytes_written;
}

int iocla_printf(const char *format, ...) {

	// check the alloc
	char *out = calloc(sizeof(char), 6000);
	if(!out)
		exit(1);

	// check the alloc
	char *string = calloc(sizeof(char), 6000);
	if(!string){
		free(out);
		exit(1);
	}

	// check the alloc
	char *aux = calloc(sizeof(char), 50);
	if(!aux){
		free(out);
		free(string);
		exit(1);
	}

	int i = 0, len = 0; // len -> number of characters printed
	long getArg = 0; // used for getting the arguments from the va_list

	va_list args;
	va_start(args, format);

	while(format[i] != '\0') {

		// if we encounter '%%%%%' string -> print only '%%' 
		if(format[i] == '%' && format[i+1] == '%'){
			i++;
			/* 
			for each '%' we need to print the next one
			so in order to do this we copy the next character in the justCopy
			block of code
			*/
			goto justCopy;
		}

		if(format[i] == '%') {
			i++;

			// int
			if(format[i] == 'd') {
				
				getArg = va_arg(args, int); // get the arg from the va_list
				itoa(getArg, aux, 10); // convert it into string with base 10
				strcpy(out + len, aux); // copy the converted string in the 'out' buffer
				len += strlen(aux); // increment the length of the 'out' string
			}
			
			// unsigned int
			else if(format[i] == 'u') {
				
				getArg = va_arg(args, int); // get the arg from the va_list
				// if parameter is negative then convert it to unsigned int
				if(getArg < 0) {
					getArg = UINT_MAX + getArg + 1;
				}

				itoa(getArg, aux, 10); // convert it into string with base 10
				strcpy(out + len, aux); //copy the converted string in the 'out' buffer
				len += strlen(aux); // increment the length of the 'out' string
			
			}

			// char
			else if(format[i] == 'c') {
				
				getArg = va_arg(args, int); // get the arg from the va_list
				out[len] = getArg; // put the char in the next index of 'out' buffer
				len++; // increment the length of the 'out' string
			}

			// hexa number
			else if(format[i] == 'x') {
				
				getArg = va_arg(args, unsigned int); // get the arg from the va_list
				itoa(getArg, aux, 16); // convert it into string with hex base
				strcpy(out + len, aux); // copy the converted string in the 'out' buffer
				len += strlen(aux); // increment the length of the 'out' string
			}
			// string
			else if(format[i] == 's') {
				
				string = va_arg(args, char*); // get the arg from the va_list
				strcpy(out + len, string); // copy the string in the 'out' buffer
				len += strlen(string); // increment the length of the 'out' string
			}
		}
		/*
		copy everything apart from the first '%'
		into the returned string
		*/
		else{
justCopy:
			out[len++] = format[i];
		}
		i++;
	}
	va_end(args);
	// write the final string -> 'out'
	write_stdout(out, len);
	// return number of characters printed
	return len;
}
