#pragma once
inline bool file_exists(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}
void GetDesktopResolution(int& horizontal, int& vertical)
{
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    horizontal = desktop.right;
    vertical = desktop.bottom;
}
char* encode(std::string plain, int key)
{
    char* encoded = new char[plain.size() + 1];
    strcpy(encoded, plain.c_str());
    for (int i = 0; i < strlen(encoded) && encoded[i] != '\0'; i++)
    {
        encoded[i] = encoded[i] + key;
    }
    return encoded;

}
std::string decode(std::string encoded, int key)
{
    char* decoded = new char[encoded.size() + 1];
    strcpy(decoded, encoded.c_str());
    for (int i = 0; i < strlen(decoded) && decoded[i] != '\0'; i++)
    {
        decoded[i] = decoded[i] - key;
    }
    return decoded;
}