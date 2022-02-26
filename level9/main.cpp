#include <iostream>

class N
{
public:
	N(int num) {this->raqm = num};
	void setAnnotation(int, int) {};
	N& operator+(N& instance);
	N& operator-(N& instance);
private:
	void	*something; // ???
	char	buffer[100];
	int 	raqm;
}

void N::setAnnotation(char *str)
{
	size_t	size;
	
	size = strlen(str);
	memcpy(this->buffer, str, size);
}

int	main(int argc, char **argv)
{
	if (argc != 0x1)
		exit(0x1);

	N *o = new N(0x5);
	N *p = new N(0x6);

	o->setAnnotation(argv[1]);

	p += o;
}
