#include<unistd.h>
#include<stdlib.h>
#include <iostream>

const std::string m_codeFolder = ".";

std::string managePipe(const std::string& str)
{
    int fd_p2c[2], fd_c2p[2];
	pid_t pid; // Pid.

	char buffer[80]; // Buffer reader.
	std::string ret = "";

	if(pipe(fd_p2c) != 0 || pipe(fd_c2p) != 0)
		std::cerr << "Failed to pipe, call your teacher\n";
	else
	{
		pid = fork(); // Creating the pipe.

		if(pid < 0)
			std::cerr << "Failed to fork(), call your teacher\n";
		else if (pid == 0) // Child process.
		{
			if (dup2(fd_p2c[0], 0) != 0 || close(fd_p2c[0]) != 0 || close(fd_p2c[1]) != 0)
				std::cerr << "Child: failed to set up standard input\n";
			else if (dup2(fd_c2p[1], 1) != 1 || close(fd_c2p[1]) != 0 || close(fd_c2p[0]) != 0)
				std::cerr << "Child: failed to set up standard output\n";
			else
			{
				execl(str.c_str(), str.c_str(), m_codeFolder.c_str(), NULL);
				std::cout << "str: " << str << "\n";
				std::cout << "m_codeFolder: " << m_codeFolder << "\n";
			}
			
			std::cerr << "Failed to execute " << str << "\n2";
			return ret;
		}
		else // Parent process.
		{
			close(fd_p2c[0]);
			close(fd_c2p[1]);
			close(fd_p2c[1]);

			int bytes_read;
			while (1)
			{
				bytes_read = read(fd_c2p[0], buffer, sizeof(buffer) -1);

				if (bytes_read <= 0)
					break;

				buffer[bytes_read] = '\0';
				ret += buffer;
			}
			close(fd_c2p[0]);
		}
	}

	return ret;
}

int main(int argc, char *argv[])
{
	std::string str;
	std::cout << ">>>: " << managePipe("/bin/ls") << "<<<\n";
	std::cin >> str;

	std::cout << ">>>: " << managePipe(str) << "<<<\n";
	/*pid_t pid;
	int estado;

	// Creamos un hijo
	if((pid=fork()) < 0)
	{
		perror("\nError en el fork");
		exit(-1);
	}
	else if(pid==0)
	{
		if( (execl("/usr/bin/ls", ".", NULL) < 0))
		{
			perror("\nError en el execl");
			exit(-1);
		}

	}

	wait(&estado);*/
	// exit(0);
}
