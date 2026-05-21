#ifndef KEY_PROCESSOR_HPP
#define KEY_PROCESSOR_HPP

class KeyProcessor {
public:
    enum class Mode {
        ORIGINAL,   
        INVERSION,  
        BLUR,    
        CANNY       
    };

private:
    Mode currentMode;

public:
    KeyProcessor();

    void processKey(int key);

    Mode getMode() const;
};

#endif