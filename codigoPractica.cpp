/***********************************************************************************************************
    Descripcion:
        Creacion de un programa que realice el conteo de los tokens de  identificador, operador, digito
        y el conteo total de tokens, desde un archivo de texto.
    Autores:
        Hernandez Lopez Bryan
        Martinez Islas Armando Samir
    Fecha de creacion:
        17/05/2022
    Version: 3.0
***********************************************************************************************************/

//Librerias
#include<bits/stdc++.h> //Libreria de cabecera.
using namespace std;

//Prototipos de las funciones.
void clean();
void leer();
int comprobar(int,int);
void mostrar();
void nombre(string a, int i, string &variable);
void puntuacion(string text);
bool isNumber(const string& str);
void identificadores();
void cuentaDeclarados();
void cuentaNuevos();
void CuentaNumeros();

//Vectores auziliares.
map<string, int> caracteres{};
unordered_map<string, string> tipo = {{"<<","Operador"}, {">>","Operador"}, {"++","Operador"}, {"--","Operador"}, {"-=","Operador"}, 
                                        {"+=","Operador"},{"*=","Operador"},{"/=","Operador"}, {"::","Operador"}, {"!","Operador"}, 
                                        {"()","Puntuacion"}, {",","Puntuacion"}, {";","Puntuacion"},  {".","Puntuacion"}, {"<","Comparador"}, 
                                        {">","Comparador"}, {"==","Comparador"}, {"<=","Comparador"}, {">=","Comparador"}, {"!=","Comparador"}, 
                                        {"+","Operador"}, {"=","Operador"}, {"-","Operador"}, {"/","Operador"}, {"*","Operador"}, {"(","Puntuacion"}, 
                                        {")","Puntuacion"}, {":","Puntuacion"}, {"//","Comentario"}, {"{","Puntuacion"}, {"}","Puntuacion"}, 
                                        {"{}","Puntuacion"}, {"/*","Comentario"}, {"*/","Comentario"}, {"length","Reservadas"}, {"size","Reservadas"}, 
                                        {"count","Reservadas"}, {"fail","Reservadas"}, {"eof","Reservadas"}, {"close","Reservadas"}, 
                                        {"push_back","Reservadas"}, {"append","Reservadas"}, {"if","Reservadas"}, {"for","Reservadas"}, {"while","Reservadas"}};

unordered_set<string> operadores = {"<<", ">>", "++", "--", "-=", "+=","*=","/=", "::", "()", "==", "<=", ">=", "!=", "+", "=", "-", "/", "*", ":","(", ")","!",",",
                                    ";",".", "<", ">", "//", "{", "}", "{}", "/*", "*/", "length", "size", "count", "fail", "eof", "close", "push_back", "append", "if", "for", "while"};
unordered_set<string> variables = {"int", "long", "double", "float","boolean", "char", "string", "auto", "ifstream", "void"};
unordered_set<string> especiales = {"length", "size", "count", "fail", "eof", "close", "push_back", "append"};
vector<string> words{};
vector<string> wordsClean{};
unordered_map<string,int> numeros{};

//Funcion principal.
int main(){
    leer();
    return 0;
}

//Funcion de lectura del archivo .txt que contiene las cadenas de caracteres.
void leer(){
    ifstream archivo;
    string contenido;
    archivo.open("input.txt",ios::in);  //Abrimos el archivo en lectura.
    if(archivo.fail()){
        cout<<"\t NO SE PUDO ABRIR EL ARCHIVO."<<endl;  //Mensaje en caso de no lograr encontrar o abrir el .txt
        exit(1);
    }
    cout<<"\n   -> Contenido del archivo de texto: "<<endl;
    cout<<"\n---------------------------------------------------------------------------------------------------------------------------"<<endl;
    while(!archivo.eof()){  //Recorremos el archivo hasta llegar al final.
        getline(archivo,contenido); //Copiamos el contenido del archivo .txt a una variable tipo string.
        cout<<"\t"<<contenido<<endl;    //Mostramos en pantalla el contenido del archivo .txt.
        puntuacion(contenido);
        identificadores();
        CuentaNumeros();
        clean();
    }
    cout<<"---------------------------------------------------------------------------------------------------------------------------\n"<<endl;
    archivo.close();    //Cerramos el archivo.
    mostrar(); //Mostramos los resultaos obtenidos.
}

//Funcion que inicializa las cadenas en las que vamos almacenando cada una de las cadenas, segun corresponda.
void clean(){
    words={};
    wordsClean={};
    numeros={};
}

//Funcion que muestra los resultados obtenidos por el programa
void mostrar(){
    int conteo=0;
    for(auto x: caracteres){
        cout<<"\t -> El token "<<x.first<<" tiene "<<x.second<<" y es de tipo: "<< tipo[x.first]<<"."<<endl;
        conteo+=x.second;
    }
    cout<<"\n\t===================================="<<endl;
    cout<<"\t -> El total de tokens es: "<<conteo<<endl;
    cout<<"\t====================================\n"<<endl;
}

//Funcion que realiza el conteo de los caracteres u operadores de puntuacion.
void puntuacion(string text){
    text+= " ";
    string space_delimiter = " ";
    words={};
    size_t pos = 0;
    while ((pos = text.find(space_delimiter)) != string::npos) {// separamos cada cadena por espacios
        words.push_back(text.substr(0, pos));
        text.erase(0, pos + space_delimiter.length());
    }
    for (const auto &str : words) { //por cada palabra
        for(const auto &strset : operadores){
            if(str.size()>=strset.size()){
                for (int i = 0; i <= (str.size() - strset.size()); i++){    //Contamos el numero de apariciones
                    //cout<<strset<<" comparando "<<str<<endl;
                    if (str.substr(i, strset.size()) == strset){
                        caracteres[strset]+=1; //incrementa en uno por cada aparicion
                    }
                }
            }   
        }
    }
}

//Funcion que comprueba si existen numeros.
bool isNumber(const string& str) {  //verificamos si es un numero
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

//Funcion que comprueba si es un numero decimal.
string isDecimal(const string& str) {   //verificamos si es un decimal
    int cont=0;
    for (char const &c : str) {
        if (std::isdigit(c) == 0){
            if(c == '.'){
                cont++;
            }
            if(c != '.')
                return "false";
        }
    }
    if(cont>0){
        return "Decimal";
    }
    return "Entero";
}

//Funcion que realiza el conteo de los identificadores
void identificadores(){
    cuentaDeclarados();
    cuentaNuevos();
}

//Funcion que realiza el conteo de las variables declaradas.
void cuentaNuevos(){
    // cout<<wordsClean.size()<<endl;
    for(int i=0; i<wordsClean.size(); i++){//por cada palabra limpia(sin caracteres especiales, previamente separada por espacios)
        if(variables.count(wordsClean[i])){//si encontramos un tipo de variable(int, float, char, etc).
            for(int j=i+1; j<wordsClean.size(); j++){
                if(!variables.count(wordsClean[j]) and !caracteres.count(wordsClean[j]) and !especiales.count(wordsClean[j]) and !isNumber(wordsClean[j])){ //agregamos esa variable
                    caracteres[wordsClean[j]] += 1;
                    tipo[wordsClean[j]] = "Identificador";//le damos el tipo ifentificador
                }
            }   
            break;
        }
    }
}

//Funcion que realzia el conteo de los numeros utilizados en el archivo.
void CuentaNumeros(){
    unordered_set<char> especiales = {'+', '-', '/', '*', ':','(', ')','!',',',';', '<', '>','=', '{', '}', '&'};
    for( auto &palabra : words){ //por cada palabra que se separo por espacios
            string newWord=palabra;
            string temp="";
            if(newWord[0]=='/' and newWord[1]=='/'){
                break;
            }
            for( int i=0; i<newWord.size(); i++){ //limpiamos esta palabra quitando caracteres excepto el . que puede tener un decimal
                if (!especiales.count(static_cast<unsigned char>(newWord[i]))){
                    temp.push_back(newWord[i]);
                }else{
                    if(temp.size()>0){
                        numeros[temp]; //contariamos la variable en caso de estar limpia
                    }
                    temp="";
                }
            }
            if(temp.size()>0){
                numeros[temp]; //contariamos la variable en caso de estar limpia
            }
    }
    for (const auto &str : numeros) {
        if(isDecimal(str.first)!="false"){//agregamos el numero a nuestra tabla hash donde hacemos el conteo
            caracteres[str.first]+=1;
            tipo[str.first]=isDecimal(str.first);
        }
    }
    
}

//Funcion que realiza el conteo de los caracteres declarados al incio del programa.
void cuentaDeclarados(){
        unordered_set<char> especiales = {'+', '-', '/', '*', ':','(', ')','!',',',';','.', '<', '>','=', '{', '}', '&'};
        for( auto &palabra : words){ //por cada palabra que se separo por espacios
            string newWord=palabra;
            string temp="";
            if(newWord[0]=='/' and newWord[1]=='/'){
                break;
            }
            for( int i=0; i<newWord.size(); i++){ //limpiamos esta palabra quitando caracteres
                if (!especiales.count(static_cast<unsigned char>(newWord[i]))){
                    temp.push_back(newWord[i]);
                }else{
                    if(temp.size()>0){
                        wordsClean.push_back(temp); //contariamos la variable en caso de estar limpia
                    }
                    temp="";
                }
            }
            if(temp.size()>0){
                wordsClean.push_back(temp); //contariamos la variable en caso de estar limpia
            }
        }
        for (const auto &str : wordsClean) {//por cada variable 
            for(const auto &strset : caracteres){//buscamos si esta declarada
                if(str == strset.first and tipo[strset.first]=="Identificador"){//si la subcadena es igual a una variabel aumentamos en 1
                    caracteres[strset.first]+=1;//aumentamos el conteo
                }
            }
        }
}
