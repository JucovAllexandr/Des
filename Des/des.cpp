#include "des.h"



void Des::encode(string key, string msg)
{
    string binaryMsg;
    binaryMsg.reserve(64);



    if(msg.size() >= 8){

        //-----Key part
        vector<string> subKeys = genSubKeys(key);

        //-----Message part
        for(int i = 0; i < 8; i++){
            cout << "Message: " << msg.at(i) << " | "<< bitset<8>(msg.at(i)) <<endl;
            binaryMsg.append(bitset<8>(msg.at(i)).to_string());
        }
        cout <<"bin msg"<< binaryMsg<<endl;
        binaryMsg = "0000000100100011010001010110011110001001101010111100110111101111";

        string IPbinMsg =  permute(binaryMsg, ip, 64);
        cout <<"initial permute "<<IPbinMsg<<endl;

        string leftIPbinMsg, rightIPbinMsg;

        leftIPbinMsg = IPbinMsg.substr(0, 32);
        rightIPbinMsg = IPbinMsg.substr(32, 32);

        cout << "left part: "<<leftIPbinMsg << endl;
        cout << "right part: "<<rightIPbinMsg << endl;

        string E = permute(rightIPbinMsg, eBit, 48);
        cout << "Expand right " << E << endl;

        cout << "Size "<<subKeys.at(0).size() << " "<<E.size() << endl;
        cout << subKeys.at(0) <<endl;
        cout << E <<endl;
        cout << "Xor: "<< Xor(subKeys.at(0), E) << endl;


    }
}

string Des::permute(string m, int *p, int size)
{
    string retPerm;
    retPerm.reserve(size);

    for(int i = 0; i < size; i++){
        retPerm += m[p[i] - 1];
    }

    return retPerm;
}

pair<string, string> Des::leftShift(string left, string right, int nbr)
{
    pair<string, string> retVecStr;

    string ltmp = left.substr(0, nbr);
    string rtmp = right.substr(0, nbr);

    retVecStr.first = left.substr(nbr, left.size());
    retVecStr.first.append(ltmp);

    retVecStr.second = right.substr(nbr, right.size());
    retVecStr.second.append(rtmp);

    return  retVecStr;
}

vector<pair<string, string> > Des::leftShift16(string left, string right)
{
    vector<pair<string, string>> retVecStr(16);
    retVecStr[0] = leftShift(left, right, 1);
    cout << 1 << " shift " << retVecStr[0].first << " | " << retVecStr[0].second << endl;

    for(int i = 1; i < 16; i++){
        if(i == 1 || i == 8 || i == 15){
            retVecStr[i] = leftShift(retVecStr[i-1].first, retVecStr[i-1].second, 1);
        }else{
            retVecStr[i] = leftShift(retVecStr[i-1].first, retVecStr[i-1].second, 2);
        }
        cout << i+1 << " shift " << retVecStr[i].first << " | " << retVecStr[i].second << endl;
    }

    return  retVecStr;
}

vector<string> Des::genSubKeys(string key)
{
    string binaryKey;
    binaryKey.reserve(56);

    for(int i = 0; i < 8; i++){
        cout << "Key: " << key.at(i) << " | "<< bitset<8>(key.at(i)) <<endl;
        binaryKey.append(bitset<8>(key.at(i)).to_string());
    }

    cout <<"bin key"<< binaryKey<<endl;
    binaryKey = "0001001100110100010101110111100110011011101111001101111111110001";

    string keyPerm = permute(binaryKey, pc1, 56);

    cout << "key permute: " << keyPerm << endl;

    string leftKey, rightKey;

    leftKey = keyPerm.substr(0, 28);
    rightKey = keyPerm.substr(28, 28);

    cout << "left key: "<<leftKey << endl;
    cout << "right key: "<<rightKey << endl;

    //pair<string, string> tmp = leftShift(leftKey, rightKey, 2);
    vector<pair<string, string>> pairs = leftShift16(leftKey, rightKey);

    vector<string> keys(16);

    for(int i = 0; i < 16; i++){
        string tmp = pairs.at(i).first + pairs.at(i).second;
        keys[i] = permute(tmp, pc2, 48);

        cout << "key "<<i+1<<" permute "<<keys[i] << endl;
    }

    return  keys;
}

string Des::Xor(string m1, string m2)
{
    string str;

    if(m1.size() == m2.size()){
        str.resize(48);

        for(int i = 0; i < m1.size(); ++i){
            if((m1.at(i) == '0' && m2.at(i) == '0') || (m1.at(i) == '1' && m2.at(i) == '1')){
                str[i] = '0';
            }else if((m1.at(i) == '1' && m2.at(i) == '0') || (m1.at(i) == '0' && m2.at(i) == '1')){
                str[i] = '1';
            }
        }
    }

    return str;
}
