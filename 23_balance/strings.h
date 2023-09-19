
int length(char* str)
{
	int i = 0;
	while (str[i] != '\0')
		i += 1;

	return i;
}

char* copy_str(char* arg)
{
	char* buff = new char[length(arg) + 1];
	strcpy(buff, arg);

	return buff;
}

char* int_to_str(int arg)
{
	char* buff = new char[8];
	int i = 0;
	while (arg > 0)
	{
		buff[i] = arg % 10 + '0';
		arg /= 10;
		i += 1;
	}

	buff[i] = '\0';
	int i2 = 0;
	i -= 1;

	while (i > i2)
	{
		swap(buff[i], buff[i2]);
		i -= 1;
		i2 += 1;
	}

	return buff;
}
char* rand_str(int size)
{
	char* buff = new char[size + 1];

	int i;
	for (i = 0; i < size; i += 1)
		buff[i] = '0' + rand() % 10;

	buff[i] = '\0';

	return buff;
}

