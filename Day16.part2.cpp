#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <limits>


struct Packet
{
    int version;
    int typeId;
    long long value;
    std::vector<Packet> packets;
};


int decodeBinary(const std::string &s)
{
    int result = 0;
    for (const auto c : s)
        result = result*2 + (c == '1');
    return result;
}


Packet decodePacket(int &pos, const std::string &binaryInput)
{
    Packet packet;
    int startPos;

    // Each packet has a three bit packet version and type id
    packet.version = decodeBinary(binaryInput.substr(pos, 3));
    pos += 3;
    packet.typeId = decodeBinary(binaryInput.substr(pos, 3));
    pos += 3;

    // Type id 4 is a literal value. Followed by any number of five-bit
    // values. The values are a continuation bit followed by four-bit
    // value
    if (packet.typeId == 4)
    {
        long long value = 0;
        while (true)
        {
            int continuationBit = binaryInput[pos++] == '1';
            int nybble = decodeBinary(binaryInput.substr(pos, 4));
            value = (value << 4) + nybble;
            pos += 4;

            if (!continuationBit)
                break;
        }
        packet.value = value;
    }

    // Other type ids are operators
    else
    {
        int lengthTypeId = decodeBinary(binaryInput.substr(pos, 1));
        ++pos;

        if (lengthTypeId == 0)
        {
            int bitLength = decodeBinary(binaryInput.substr(pos, 15));
            pos += 15;

            int subPos{0};
            std::string subInput = binaryInput.substr(pos, bitLength);
            while (subPos < subInput.size())
            {
                auto subPacket = decodePacket(subPos, subInput);
                packet.packets.emplace_back(subPacket);
            }
            pos += bitLength;
        }
        else
        {
            int subPacketCount = decodeBinary(binaryInput.substr(pos, 11));
            pos += 11;

            for (int i = 0; i < subPacketCount; i++)
            {
                auto subPacket = decodePacket(pos, binaryInput);
                packet.packets.emplace_back(subPacket);
            }
        }
    }

    return packet;
}



long long calculatePacket(const Packet &packet)
{
    switch (packet.typeId)
    {
    case 0:    // Sum
        {
        long long res{0};
        for (const auto &p : packet.packets)
            res += calculatePacket(p);
        return res;
        }

    case 1:    // Product
        {
        long long res{1};
        for (const auto &p : packet.packets)
            res *= calculatePacket(p);
        return res;
        }

    case 2:    // Min
        {
        long long res{std::numeric_limits<long long>::max()};
        for (const auto &p : packet.packets)
        {
            auto v = calculatePacket(p);
            if (v < res) res = v;
        }
        return res;
        }

    case 3:    // Max
        {
        long long res{std::numeric_limits<long long>::min()};
        for (const auto &p : packet.packets)
        {
            auto v = calculatePacket(p);
            if (v > res) res = v;
        }
        return res;
        }

    case 4:    // Literal
        return packet.value;

    case 5:    // Greater than
        return calculatePacket(packet.packets[0]) > calculatePacket(packet.packets[1]);

    case 6:    // Less than
        return calculatePacket(packet.packets[0]) < calculatePacket(packet.packets[1]);

    case 7:    // Equal
        return calculatePacket(packet.packets[0]) == calculatePacket(packet.packets[1]);

    default:
        return 0;
    }
}



int main(int, char **)
{
    // Read hex string
    std::string hexInput;
    std::cin >> hexInput;

    // Convert to binary string
    const std::map<char, std::string> hexToBin =
    {
        {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"},
        {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"},
        {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"},
        {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"},
    };

    std::string binaryInput;
    for (const auto c : hexInput)
    {
        binaryInput.append(hexToBin.at(c));
    }


    // Parse string into packet
    int pos = 0;
    Packet packet = decodePacket(pos, binaryInput);

    std::cout << "Result = " << calculatePacket(packet) << "\n";

    return 0;
}

