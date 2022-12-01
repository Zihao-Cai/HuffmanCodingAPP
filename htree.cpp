#include "htree.h"

Htree::Htree()
{

}

Point* Htree::gettree(){
    return tree;
}

bool Htree::create(QFile *f){
    if(!f->open(QIODevice::ReadOnly))   return false;
    //读取数据
    Point* temptree = new Point[128*sizeof(Point)]();
    char* buffer = new char[100]();
    int res = -1;
    int n = 0;//记录有效字符数
    do{
        res = f->readLine(buffer,100);//读取一行
        if(res!=-1){
            for(int i=0;i<res;i++){
                int j=0;
                for(j=0;j<n;j++){
                    if(*(buffer+i)==(temptree+j)->getdata())    break;
                }
                if(j == n) { (temptree+n)->setdata(*(buffer+i));(temptree+n)->setweight((temptree+n)->getweight()+1);n++; }
                else{(temptree+j)->setweight((temptree+j)->getweight()+1); }
            }
        }
    }while(res!=-1);
    //文件读取完毕
    f->close();
    delete [] buffer;

    this->tree = new Point[(2*n)*sizeof(Point)]();//为树开辟等量大小的存储空间,n个叶子结点需要2n-1个结点,这里分配2n个空间，使用1到2n-1下标
    for(int k=0;k<n;k++){
        (tree+k+1)->setdata((temptree+k)->getdata());
        (tree+k+1)->setweight((temptree+k)->getweight());
    }
    delete[] temptree;//拷贝后释放临时存储

    for(int i=n+1;i<=2*n-1;i++){
        int lowest=1;int lower=2;
        select(tree,i-1,lowest,lower);//选中两个最小权的叶子后,与当前i结点建立父子关系//
        (tree+i)->setlchild(tree+lowest);
        (tree+i)->setrchild(tree+lower);
        (tree+i)->setweight((tree+lowest)->getweight()+(tree+lower)->getweight());
        (tree+lowest)->setparent(tree+i);
        (tree+lower)->setparent(tree+i);
    }   //of for合并完成,树构建完毕
    //每个叶子结点向上溯根,获得对应字符编码
    Element elements[n+1];//存储从1到n的有效字符和它的编码
    int wordsize = 0;
    char* buf = new char[n]();
    for(int r=1;r<=n;r++){
        elements[r].char_ = (tree+r)->getdata();
        *(buf+n-1) = '\0';
        int loc = n-1;
        Point* chi = (tree+r);Point* par = (tree+r)->getparent();
        while(par!=nullptr){
            loc--;
            if(chi == par->getlchild()) *(buf+loc) = '0';
            else    *(buf+loc) = '1';
            chi = par;par = par->getparent();
        }//of while 该字符回溯完毕
        elements[r].code = new char[n-loc]();
        strcpy(elements[r].code,buf+loc);
        elements[r].codelength = (int)strlen(elements[r].code);
        wordsize += (tree+r)->getweight()*elements[r].codelength;
    }   //of for 所有字符回溯完毕
    delete [] tree;
    delete[] buf;   //所有编码均已存放在elements数组内,释放临时存储

    //下一步要将编码压缩后写入文件:
    QString new_filename = QString("C:/Users/蔡子豪/Desktop/Hufcod.dat");
    QFile newfile(new_filename);
    newfile.open(QIODevice::WriteOnly);//以只写方式打开新文件,不存在则创建
    newfile.close();

    if(!newfile.open(QIODevice::WriteOnly))    return false;
    //开头写入字符编码表,待解码文件时使用
    int rearlack = 8-wordsize%8;
    int bytenum = wordsize/8;//这里没有考虑尾部冗余部分,届时单独处理
    char* word = new char[wordsize]();
    QDataStream stream(&newfile);
    char* sample = new char[8]();
    char* byte_writer = new char();
    QByteArray ba = QString::number(n,2).toLatin1();
    sample = ba.data();
    str_makeup(sample); //该方法将不足八位的二进制字符补足到八位对齐格式
    for(int j=0;j<8;j++){
        if(*(sample+j) == '1')  *byte_writer |= (1<<(7-j));
        else    *byte_writer &= ~(1<<(7-j));
    }
    if((stream.writeRawData(byte_writer,1)) == -1)
    {   //这里将字符条目数量转化为了二进制写入文件开头
        newfile.close();
        return false;
    }

    //write wordsize
    ba = QString::number(wordsize,2).toLatin1();
    char* sizebox = new char[32]();
    int len = (int)strlen(ba.data());
    if(len != 32){
        for(int k=len-1;k>=0;k--){
        *(sizebox+k+32-len) = *(ba.data()+k);
        }
        for(int k=0;k<32-len;k++){
        *(sizebox+k) = '0';
        }
    }//将sizebox补足32位
    for(int k=0;k<4;k++){
        for(int j=k*8;j<k*8+8;j++){
            *(sample+j%8) = *(sizebox+j);
        }
        for(int j=0;j<8;j++){
            if(*(sample+j) == '1'){
                *byte_writer |= (1<<(7-j));
            }else{
                *byte_writer &= ~(1<<(7-j));
            }
        }
        if(!(stream.writeRawData(byte_writer,1)))   return false;
    }
    //write wordsize done

    //下面写入3n条编码表项,格式为:   字符码长编码字符码长编码...
    for(int k=1;k<=n;k++){
        if(stream.writeRawData(&elements[k].char_,1) == -1)    return false;//写入字符
        *byte_writer = (char) elements[k].codelength;
        if(stream.writeRawData(byte_writer,1) == -1)   return false;//写入码长
        str_makeup(elements[k].code);
        *byte_writer = '\0';
        for(int j=0;j<8;j++){
            if(*(elements[k].code+j) == '1'){
                *byte_writer |= (1<<(7-j));
            }else{
                *byte_writer &= ~(1<<(7-j));
            }
        }//of for j->byte_8
        if(stream.writeRawData(byte_writer,1) == -1)    return false;//写入字符对应编码
    }//of for k->n 所有字符及其编码作为文件头写入完
    newfile.close();

    //下面写入原文本内容
    //进行第二次读文件,读取每个字符用编码格式存入word
    int cont = 0;
    if(!f->open(QIODevice::ReadOnly))   return false;
    QDataStream stream_(f);
    while(cont<wordsize){
        stream_.readRawData(byte_writer,1);
        int r =1;
        for(r=1;r<=n;r++){
        if(elements[r].char_ == *byte_writer){
            break;
        }
        }//找到对应的字符表项r
        int templen = elements[r].codelength;
        for(int q = 0;q<templen;q++){
            *(word+cont) = *(elements[r].code+q+8-templen);
            cont++;
        }//将读到的单个字符编码后存入word
    }//of while 文件全部读取完毕并编码存入word
    f->close();

    if(!newfile.open(QIODevice::Append)) return false;
    cont = 0;
    for(int p=0;p<bytenum;p++){
        for(int j=0;j<8;j++){
            *(sample+j) = *(word+cont);
            cont++;
        }//每次取word中的8位,用sample做位运算由8byte变成1byte写入f,共取bytenun次
        *byte_writer = '\0';
        for(int j=0;j<8;j++){
            if(*(sample+j) == '1')  *byte_writer |= (1<<(7-j));
            else    *byte_writer &= ~(1<<(7-j));
        }//做位运算
        if((stream.writeRawData(byte_writer,1)) == -1)  return false;
    }
    //下面处理尾部:
    if(rearlack != 0){  //冗余部分须补足八位,在短码后面补上rearlack个0即可
        int k = 0;
        for(k=0;k<8-rearlack;k++){
            *(sample+k) = *(word+cont);
            cont++;
        }
        for(;k<8;k++){
            *(sample+k) = '0';
        }
        *byte_writer = '\0';
        for(int j=0;j<8;j++){
            if(*(sample+j) == '1')  *byte_writer |= (1<<(7-j));
            else    *byte_writer &= ~(1<<(7-j));
        }//做位运算
        if((stream.writeRawData(byte_writer,1)) == -1)  return false;
    }//尾部处理完成
    //压缩文本内容写入完
    delete [] word;
    delete byte_writer;
    newfile.close();
    return true;
}



bool Htree::select(Point *tree, int upon, int &indexlowest, int &indexlower){

    int lowest = 1000000;
    int lower = 1000000;
    for(int k=1;k<=upon;k++){
        if((tree+k)->getparent() != nullptr){continue;}
        else{
            if((tree+k)->getweight()<lowest){lower = lowest;indexlower = indexlowest;lowest = (tree+k)->getweight();indexlowest = k;}
            else if((tree+k)->getweight()<lower && k!=indexlowest){lower = (tree+k)->getweight();indexlower = k;}
        }
    }       //printf("%d\t%d\n",indexlowest,indexlower);
    return true;
}



bool Htree::decode(QFile *f){
    if(!f->open(QIODevice::ReadOnly))   return false;
    int wordsize = 0;
    QDataStream stream(f);
    char* byte_reader = new char();
    char* sample = new char[8]();
    if((stream.readRawData(byte_reader,1)) == -1) return false;           //将压缩后的字符逐一读取出来,参数len表示每次读取的字节
    for(int k=0;k<8;k++){
        if((*byte_reader & 10000000) == 10000000){//位运算检验:低7位结果全部置为0,满足条件则证明高位即第一位为1
            *(sample+k) = '1';
        }else{
            *(sample+k) = '0';
        }
        *byte_reader <<= 1;                       //每位运算一次左移一位
    }                                       //of for 循环结束,该字符转换为二进制存放在sample内
    int n = 0;                              //从文件头读取的首个数据块就是压缩的字符条目数量
    for(int i=(int)strlen(sample)-1;i>=0;i--){
        if(*(sample+i) == '1') n += pow(2,7-i);
    }                                       //n读取并解码完成
    //read wordsize
    char* sizebox = new char[32]();
    for(int k=0;k<4;k++){
        if(stream.readRawData(byte_reader,1) == -1) return false;
        for(int j=0;j<8;j++){
            if((*byte_reader & 10000000) == 10000000){
                *(sample+j) = '1';
            }else{
                *(sample+j) = '0';
            }
            *byte_reader <<= 1;
        }
        for(int j=8*k;j<8*k+8;j++){
            *(sizebox+j) = *(sample+j%8);
        }
    }
    for(int i=31;i>=0;i--){
        if(*(sizebox+i) == '1') wordsize += pow(2,31-i);
    }
    //read wordsize done

    Element elements[n+1];//存储从1到n的有效字符和它的编码,注意这里存储编码时采用不等长所以要依据码长将补足码删减复原
    for(int k=1;k<=n;k++){
        if(stream.readRawData(byte_reader,1) == -1) return false;//读取每条字符
        elements[k].char_ = *byte_reader;
        if(stream.readRawData(byte_reader,1) == -1) return false;//读取码长
        for(int j=0;j<8;j++){//解压码长
            if((*byte_reader & 10000000) == 10000000){
                *(sample+j) = '1';
            }else{
                *(sample+j) = '0';
            }
            *byte_reader <<= 1;
        }
        for(int i=7;i>=0;i--){
            if(*(sample+i) == '1') elements[k].codelength += pow(2,7-i);
        }    //码长解压完成
        elements[k].code = new char[elements[k].codelength]();//为编码分配内存空间,这一步省略将导致段错误
        if(stream.readRawData(byte_reader,1) == -1) return false;//读取每个字符对应压缩编码
        for(int j=0;j<8;j++){                 //解压字符编码
            if((*byte_reader & 10000000) == 10000000){
                *(sample+j) = '1';
            }else{
                *(sample+j) = '0';
            }
            *byte_reader <<= 1;               //每位运算一次左移一位
        }//of for j->8_byte
        for(int l = 0,r = 8-elements[k].codelength;l<elements[k].codelength;l++,r++){
            *(elements[k].code+l) = *(sample+r);
        }
    }//of for k->n  字符编码表读取解压完毕

    int rearlack = 8-wordsize%8;
    int bytenum = wordsize/8;//这里没有考虑尾部冗余部分,届时单独处理
    char* word = new char[wordsize]();
    int cont = 0;
    for(int r=0;r<bytenum;r++){
        if(stream.readRawData(byte_reader,1) == -1) return false;
        for(int j=0;j<8;j++){
            if((*byte_reader & 10000000) == 10000000){
                *(sample+j) = '1';
            }else{
                *(sample+j) = '0';
            }
            *byte_reader <<= 1;
        }//每次读取1字节压缩码并解压,临时存在sample内,然后在下次循环前将其转移至word内
        for(int j=0;j<8;j++){
            *(word+cont) = *(sample+j);
            cont++;
        }
    }
    //然后处理尾部冗余部分,有冗余则处理
    if(rearlack != 0){
        if(stream.readRawData(byte_reader,1) == -1) return false;//有冗余则读取出最后一个字节
        for(int j=0;j<8;j++){
            if((*byte_reader & 10000000) == 10000000){
                *(sample+j) = '1';
            }else{
                *(sample+j) = '0';
            }
            *byte_reader <<= 1;
        }//解压最后一个字节
        for(int j=0;j<8-rearlack;j++){
            *(word+cont) = *(sample+j);//取走实际长度8-rearlack的编码
            cont++;
        }
    }// of for ->wordsize  f文件内容全部读取完毕
    f->close();

    //接下来依据elements编码表将word逐次转换成原字符,同时写入解压新文件
    QString decpath = QString("C:/Users/蔡子豪/Desktop/Hufdec.dat");
    QFile decfile(decpath);
    if(!decfile.open(QIODevice::WriteOnly)) return false;
    decfile.close();//建立解压文件

    if(!decfile.open(QIODevice::WriteOnly)) return false;
    QDataStream st(&decfile);





    decfile.close();
    return true;
}


bool Htree::str_tobin(char* data){
    if(*data == '\0')   return false;
    int k=0;
    do{
        if(*(data+k)!='0' || *(data+k)!='1')    return false;


        k++;
    }while(*(data+k)!='\0');
    return true;
}


char* Htree::str_makeup(char* &src){
    int len = (int)strlen(src);
    if(len == 8)   return src;
    for(int k=len-1;k>=0;k--){
        *(src+k+8-len) = *(src+k);
    }
    for(int k=0;k<8-len;k++){
        *(src+k) = '0';
    }
    return src;
}






