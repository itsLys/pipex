#include <fcntl.h>
#include <unistd.h>

int main()
{


	int fd = open("f1", O_RDONLY);
	close(fd);

	return (0);
}
