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

        string IPbinMsg =  permute(binaryMsg, IP, 64);
        cout <<"initial permute "<<IPbinMsg<<endl;

        string leftIPbinMsg, rightIPbinMsg;

        leftIPbinMsg = IPbinMsg.substr(0, 32);
        rightIPbinMsg = IPbinMsg.substr(32, 32);

        cout << "L0: "<<leftIPbinMsg << endl;
        cout << "R0: "<<rightIPbinMsg << endl;

        //feistelNetwork(rightIPbinMsg, subKeys.at(0));

        vector<pair<string, string>> _lr;
        _lr.resize(17);

        _lr[0].first = leftIPbinMsg;
        _lr[0].second = rightIPbinMsg;

        for(int i = 1; i <= 16; i++){
            _lr[i].first = _lr[i-1].second;
            _lr[i].second = Xor(_lr[i-1].first, feistelNetwork(_lr[i-1].second, subKeys.at(i-1)));

            cout <<"L"<<i<<": "<<_lr[i].first<<endl;
            cout <<"R"<<i<<": "<<_lr[i].second<<endl;
        }

        string L = _lr.at(_lr.size()-1).first;
        string R = _lr.at(_lr.size()-1).second;

        string finMsg = R;
        finMsg.append(L);
        /*for(int i = 0; i < 32; i+=4){
            finMsg.append(R.substr(i,4) + L.substr(i,4));
        }*/

        cout << "Union rl "<<finMsg<<endl;

        finMsg = permute(finMsg, FP, 64);

        cout << "Fin permute "<<finMsg<<endl;

        cout<< "M = ";
        for(int i = 0; i < finMsg.size(); i+=8){
            cout<<hex<<bitset<8>(finMsg.substr(i,8)).to_ulong();
        }
        cout<<endl;
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

    string keyPerm = permute(binaryKey, PC1, 56);

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
        keys[i] = permute(tmp, PC2, 48);

        cout << "key "<<i+1<<" permute "<<keys[i] << endl;
    }

    return  keys;
}

string Des::Xor(string m1, string m2)
{
    cout<<"Xor "<<m1<<" "<<m2<<endl;
    string str;

    if(m1.size() == m2.size()){
        str.resize(m1.size());

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

string Des::s_function(string str)
{
    string retStr;
    if(str.size() == 48){
        retStr.reserve(32);
        vector<string> splitBy6Bit;

        for(int i = 0; i < str.size(); i+=6){
            splitBy6Bit.push_back(str.substr(i, 6));
        }

        /*for(int i = 0; i< splitBy6Bit.size();i++){
            cout << "Split str "<< splitBy6Bit.at(i)<<endl;
        }*/

        unsigned long d_row, d_col;
        string s_row, s_col;
        s_row.resize(2);

        for(int si = 0; si < 8; si++){
            s_row[0] = splitBy6Bit.at(si).at(0);
            s_row[1] = splitBy6Bit.at(si).at(5);

            //cout << "Fist and last bits "<<s_row<<endl;
            d_row = bitset<2>(s_row).to_ulong();

            //cout<< "Decimal representation "<< d_row<<endl;
            s_col = splitBy6Bit.at(si).substr(1,4);

            //cout << "4 middle bits "<<s_col<<endl;
            d_col = bitset<4>(s_col).to_ulong();

            //cout<< "Decimal representation "<< bitset<4>(s_col).to_ulong()<<endl;
            //cout<< "S"<<si<<" "<<S[si][d_row][d_col]<<" | "<<bitset<4>(S[si][d_row][d_col])<<endl;
            retStr.append(bitset<4>(S[si][d_row][d_col]).to_string());
        }
    }

    return retStr;
}

string Des::feistelNetwork(string r, string k)
{
    string retStr;

    if(r.size() == 32 && k.size() == 48){
        string E = permute(r, eBit, 48);
        /*cout << "Expand right " << E << endl;

        cout << "Size "<<k.size() << " "<<E.size() << endl;
        cout << k <<endl;
        cout << E <<endl;*/

        string _xor = Xor(k, E);
        //cout << "Xor: "<< _xor << endl;

        string sf = s_function(_xor);
        //cout << "S function: "<<endl<< sf<<endl;

        retStr = permute(sf, P, 32);
        //cout<<"permute s func "<<retStr<<endl;
        cout<<"f = "<< retStr<< endl;
    }

    return retStr;
}
