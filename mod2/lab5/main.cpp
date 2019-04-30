////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "Huffman.h"

#include <iostream>
#include <bitset>
#include <vector>
#include <queue>
#include <assert.h>

//////////////////////////////////////////////////////////////////////////////////////////////////
// Обертка над IInputStream и IOutStream

class BitReader {
    enum flags {READING, END_READING};

public:
    BitReader(IInputStream &inputStream):
            inputStream(inputStream),
            cache(0),
            bitCount(0),
            mode(READING){}

    bool ReadBit(byte &bit) {
        assert(bitCount >= 0 && bitCount <= 24);

        readFromStream();
        if ( bitCount == 0 )
            return false;

        bitCount--;
        size_t value = cache >> bitCount;
        bit = (byte)value;
        cache %= (1 << bitCount);
        return true;
    }

    bool ReadByte(byte &value) {
        assert(bitCount >= 0 && bitCount <= 24);

        readFromStream();
        if ( bitCount == 0 )
            return false;
        if ( bitCount  < 8 )
            throw "Invalid read byte, low cache";

        bitCount -= 8;
        size_t tmp = cache >> bitCount;
        value = (byte)tmp;
        cache %= (1 << bitCount);
        return true;
    }

private:
    IInputStream &inputStream;
    size_t cache;
    size_t bitCount;
    flags  mode;

    void readFromStream() {
        // храним как минимум 3 байта входного потока
        while ( bitCount <= 16 && mode == READING) {
            byte value;

            // если чтение идет, то записываем байт в кеш
            if ( inputStream.Read(value) ){
                cache = (cache << 8) | value;
                bitCount += 8;
            } else {    // иначе обрабатываем последний входной байт кеша
                        // так как в нем сохранен номер последних значащих
                        // бит в декодировании (прим. если 2 то 7 и 6 биты)
                if (bitCount >= 16) {   // это условие необходимо, чтобы проверить
                                        // не пуст ли поток данных, переданных для чтения
                                        // минимальный размер сжатого файла должен быть
                                        // не менее 3 байт (дерево + поток кодированных
                                        // символов + последний байт)
                                        // если условие не выполняется, то файл пуст или
                                        // невалиден
                    size_t tailBits = cache % 8;
                    cache   >>= 8;
                    bitCount -= 8;

                    if ( tailBits != 0 ) {
                        cache   >>= 8 - tailBits;
                        bitCount -= 8 - tailBits;
                    }
                }
                mode = END_READING;
            }
        }
    }
};


class BitWriter
{
public:
    BitWriter(IOutputStream &outputStream): outputStream(outputStream), cache(0), bitCount(0) {}

    void WriteBit(byte bit){
        assert(bit == 0 || bit == 1);
        assert(bitCount >= 0 && bitCount < 8);

        cache = (cache << 1) + bit;
        bitCount++;
        writeToStream();
    }

    void WriteByte(unsigned char byte){
        assert(bitCount >= 0 && bitCount < 8);

        cache = (cache << 8) | byte;
        bitCount += 8;
        writeToStream();
    }

    void WriteCacheRemainder(){
        assert(bitCount >= 0 && bitCount < 8);

        if ( bitCount > 0 ) {
            size_t value = cache << (8 - bitCount);
            outputStream.Write((byte)value);
            cache    = 0;
        }
        outputStream.Write((byte)bitCount);
    }

private:
    IOutputStream &outputStream;
    size_t cache;
    size_t bitCount;

    void writeToStream(){
        if ( bitCount >= 8 ){
            bitCount -= 8;
            size_t value = cache >> bitCount;
            outputStream.Write((byte)value);
            cache %= (1 << bitCount);
        }
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////\

class Huffman {
    struct Code {
        size_t code;
        size_t bitsCount;
    };

    struct Node {
        byte ch;
        Node *left,*right;

        explicit Node(): ch(0), left(nullptr), right(nullptr){}
        explicit Node(byte ch): ch(ch), left(nullptr), right(nullptr) {}
    };

    struct NodeWeight {
        Node  *node;
        size_t weight;

        NodeWeight(Node *node, size_t weight): node(node), weight(weight){}
    };

    class CompareGreaterWeight {
    public:
        bool operator()(const NodeWeight &left, const NodeWeight &right) {
            return left.weight > right.weight;
        }
    };

    using priority_queue = std::priority_queue<NodeWeight, std::vector<NodeWeight>, CompareGreaterWeight>;

public:
    Huffman(): root(nullptr), symbolsCodes(256) {}

    void BuildCodesVector( std::vector<byte> original ) {
        std::vector<size_t> freqSymbolsAnalisis(256, 0);
        buildFrequencySymbolAnalisis(original, freqSymbolsAnalisis);

        priority_queue priorityQueueSymbols;
        buildPrioritySymbolsQueue(freqSymbolsAnalisis, priorityQueueSymbols);

        buildTreeOnQueue(priorityQueueSymbols);
        buildSymbolsCodes();
    }

    void BuildTreeFromStream( BitReader &br){
        buildTreeFromStream(br, root);
    }

    void SaveTreeToStream( BitWriter &bw ){
        saveTreeToStream(bw, root);
    }

    size_t GetCodeFromVector(byte ch) {
        return symbolsCodes[ch].code;
    }

    size_t GetCodeBitsFromVector(byte ch){
        return symbolsCodes[ch].bitsCount;
    }

    bool ReadChar( BitReader &br, byte &ch){
        assert( root );

        Node *node = root;
        while ( node->left && node->right ) {
            byte bit;

            if ( !br.ReadBit(bit) )
                return false;
            if ( bit == 0 ) {
                node = node->left;
            } else if ( bit == 1 ) {
                node = node->right;
            } else {
                assert(false);
            }
        }

        if ( !node->left && !node->right ){
            ch = node->ch;
            return true;
        } else {
            assert(false);
        }
    }


private:
    Node *root;
    std::vector<Code> symbolsCodes;  // коды символов и размерность кода в битах

    void buildFrequencySymbolAnalisis(
            std::vector<byte> &original,
            std::vector<size_t> &freqSymbolsAnalisis)
    {
        unsigned char byte;
        for (size_t i = 0; i < original.size(); i++) {
            byte = original[i];
            freqSymbolsAnalisis[byte]++;
        }
    }

    void buildPrioritySymbolsQueue(
            std::vector<size_t> &freqSymbolsAnalisis,
            priority_queue &priorityQueueSymbols)
    {
        for (size_t ch = 0; ch < 256; ch++ ) {
            if ( freqSymbolsAnalisis[ch] > 0 ) {
                Node *node = new Node(ch);
                NodeWeight newNodeWeight(node, freqSymbolsAnalisis[ch]);
                priorityQueueSymbols.push(newNodeWeight);
            }
        }
    }

    void buildTreeOnQueue(priority_queue &priorityQueueSymbols ) {
        assert( !priorityQueueSymbols.empty() );

        while (priorityQueueSymbols.size() != 1) {
            NodeWeight left = priorityQueueSymbols.top();
            priorityQueueSymbols.pop();
            NodeWeight right = priorityQueueSymbols.top();
            priorityQueueSymbols.pop();

            size_t weight = left.weight + right.weight;
            Node *node = new Node();
            node->left  = left.node;
            node->right = right.node;

            NodeWeight newNodeWeight(node, weight);

            priorityQueueSymbols.push(newNodeWeight);
        }

        NodeWeight lastNodeWeight = priorityQueueSymbols.top();
        root = lastNodeWeight.node;
        priorityQueueSymbols.pop();

        assert(priorityQueueSymbols.empty());
    }

    void buildSymbolsCodes() {
        if ( !root->left && !root->right ) {
            symbolsCodes[root->ch].code = 0;
            symbolsCodes[root->ch].bitsCount = 1;
        } else {
            buildCodes(root, 0, 0);
        }
    }

    void buildCodes(Node *node, size_t code, size_t bitsCount) {
        if ( !node->left && !node->right ) {
            symbolsCodes[node->ch].code      = code;
            symbolsCodes[node->ch].bitsCount = bitsCount;
        } else if ( node->left && node->right ){
            bitsCount++;
            buildCodes(node->left,  code << 1    , bitsCount);
            buildCodes(node->right, code << 1 | 1, bitsCount);
        } else {
            throw "Invalid node";
        }
    }

    void buildTreeFromStream( BitReader &br, Node *&node) {
        byte bit = 0;
        br.ReadBit(bit);

        if ( bit == 0 ) {
            node = new Node();
            buildTreeFromStream(br, node->left);
            buildTreeFromStream(br, node->right);
        } else if ( bit == 1) {
            byte value;
            br.ReadByte(value);
            node = new Node(value);
        } else {
            assert(false);
        }
    }

    void saveTreeToStream(BitWriter &bw, Node *node){
        assert((node->left && node->right) ||
               (!node->left && !node->right));

        if ( node->left && node->right ) {
            bw.WriteBit(0);
            saveTreeToStream(bw, node->left);
            saveTreeToStream(bw, node->right);
        } else if ( !node->left && !node->right ) {
            bw.WriteBit(1);
            bw.WriteByte(node->ch);
        } else {
            assert(false);
        }
    }

};

void Encode(IInputStream& original, IOutputStream& compressed){

    std::vector<byte> cpOriginal;

    Huffman hm;
    BitWriter bw(compressed);

    byte value;
    while (original.Read(value))
        cpOriginal.push_back(value);

    if ( cpOriginal.empty() )
        return;

    hm.BuildCodesVector(cpOriginal);
    hm.SaveTreeToStream(bw);

    value = 0;
    for (size_t i = 0; i < cpOriginal.size(); i++){
        value = cpOriginal[i];

        size_t code = hm.GetCodeFromVector(value);
        size_t bits = hm.GetCodeBitsFromVector(value);

        while (bits) {
            bits--;
            byte bit = (byte)(code >> bits);
            bw.WriteBit(bit);
            code %= (1 << bits);
        }
    }
    bw.WriteCacheRemainder();
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    Huffman hm;
    BitReader br(compressed);

    hm.BuildTreeFromStream(br);

    byte ch;
    while ( hm.ReadChar(br, ch) )
        original.Write(ch);
}