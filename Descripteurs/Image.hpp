#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <sstream>

class Image {
    private:
        std::string source;
        std::string titre;
        int numero;
        double prix;
        char acces;
        std::string type;

    public:
        Image(const std::string& src, const std::string& tit, int num, double prx, char acc, const std::string& typ);
        std::string getDescripteur() const;
        int getNumero() const;
};

#endif
