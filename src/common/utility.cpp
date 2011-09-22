#include "utility.h"

logger *logger::l_singleton = NULL;

bool file_exists(string filename)
{
    struct stat sb;
    stat(filename.c_str(), &sb);

    if (errno == ENOENT || (sb.st_mode & S_IFMT) != S_IFREG)
    return false;

    return true;
}

bool dir_exists(string dirname)
{
    struct stat sb;
    stat(dirname.c_str(), &sb);

    if (errno == ENOENT || (sb.st_mode & S_IFMT) != S_IFDIR)
    return false;

    return true;
}

void addslash(string &str)
{
     if (str.at(str.length()-1) != '/')
        str += "/";
}

char* read_file(const char *filename)
{
    ifstream file;
    char   *buffer = NULL;
    int length = 0;

    file.open(filename, ios::binary);

    if (!file.is_open())
    {
        printf("file: %s\n", filename);
        perror("open()");
        return 0;
    }

    file.seekg (0, ios::end);
    length = file.tellg();
    file.seekg (0, ios::beg);

    buffer = new char[length + 1];

    file.read (buffer,length);
    file.close();
    buffer[length] = 0;
    return buffer;
}

uint32 get_columns()
{
    char *str = getenv("COLUMNS");
    if (strcmp(str, ""))
        return 0;
    return atoi(str);
}

// possibile aggiunta di bool echo che nasconde l'output
int exec_command(const char *fmt, ...)
{
    char buffer[CMAXSIZE];
    int ret;

    va_list ap;
    va_start(ap, fmt);
    ret = vsnprintf(buffer, CMAXSIZE, fmt, ap);
    va_end(ap);

    return system(buffer);
}
