# AdvancedDataStructures
This repository was created to hold Assignments from CKP8077 - Advanced Data Sctructures at UFC.


Credits to https://www.programiz.com/dsa/red-black-tree to help implementing the standart red-black-tree.

I added the persistency as the assignment asked for it.
# How to use this repo:

```cd PersistentRedBlackTree```

To compile on windows: ```g++ -o PersistentRedBlackTree.exe PersistentRedBlackTree.cpp -g && ./PersistentRedBlackTree.exe```

To compile on linux/mac: ```g++ -o PersistentRedBlackTree.out PersistentRedBlackTree.cpp -g && ./PersistentRedBlackTree.out```

treeExample.txt is the file containing the instructins to the tree.

# Assignment description (In portuguese):

**Objetivo:** Implementar a seguinte estrutura de dados com persistência parcial:

Graduação: lista encadeada ordenada.
Pós-graduação: árvore rubro-negra.

O seu programa deve receber como entrada um arquivo de texto com um conjunto de operações e deve escrever como saída um arquivo de texto com o resultado das operações. 

Versões da estrutura: As versões da estrutura serão identificadas por inteiros não negativos onde a estrutura inicial vazia terá valor 0. Cada operação de inclusão ou remoção cria uma nova versão identificada pelo próximo inteiro da versão modificada pela operação.

Formato do arquivo de entrada:
Cada linha do arquivo de entrada contém exatamente uma operação como descrita a seguir.

**Inclusão:**
Uma operação de inclusão será descrita por "INC" seguida de um espaço e depois um inteiro. Este elemento deve ser incluído na sua estrutura de dados.
Exemplo de linha de inclusão:
INC 13

**Remoção:**
Uma operação de remoção será descrita por "REM" seguida de um espaço e depois um inteiro. Um nó com este valor deve ser removido (apenas um). Caso não tenha nenhum nó com este valor, a estrutura não será modificada, mas um novo identificador da estrutura será criado.
Exemplo de linha de remoção:
REM 42

**Sucessor:**
Uma operação de sucessor será descrita por "SUC" seguida de dois inteiros, separados por espaços. O primeiro inteiro será o valor para obter o sucessor e o segundo inteiro será a versão da estrutura em que a operação de sucessor será realizada. Caso esta versão não exista, a operação deve ser realizada na última versão da estrutura. O sucessor de um valor x será o menor valor que tem na estrutura com valor estritamente maior que x, e infinito (INF) caso não haja valor maior que x na estrutura. Deve ser escrito no arquivo de saída a linha da operação realizada seguida por uma linha com o resultado da operação.
Exemplo de linha de sucessor:
SUC 50 65
Exemplo no arquivo de saída:
SUC 50 65
52

**Imprimir:**
Uma operação de impressão será descrita por "IMP" seguida de um espaço e um inteiro. O inteiro indica a versão da estrutura que deve ser impressa e, caso esta versão não exista, a operação deve ser realizada na última versão da estrutura. O arquivo de saída deve ter uma cópia da operação de impressão seguido por uma linha com a a estrutura impressa. Para a lista encadeada, basta imprimir os valores em ordem, separados por espaços. Para a árvore rubro-negra, os valores devem ser impressos em ordem e cada valor será seguido da sua profundidade na árvore e pela sua cor, separados por vírgulas.
Exemplo de linha de impressão:

IMP 65
Exemplo no arquivo de saída para lista encadeada:
IMP 65
13 42 50 52 65

Exemplo no arquivo de saída para árvore rubro-negra:
IMP 65
13,2,R 42,1,N 50,0,N 52,2,R 65,1,N


**Observação sobre a implementação.**
- A implementação deve utilizar o método descrito em sala de aula.
- Assumir que serão realizadas no máximo 99 operações de inclusão ou remoção, para que os identificadores das versões possam ser salvos em um vetor de tamanho 100 (com a versão inicial).
- O número de operações de sucessor ou impressão são ilimitados.
- Os alunos da graduação podem fazer o trabalho em grupos de até 5 alunos.
- Os alunos da pós-graduação podem fazer o trabalho em grupos de até 2 alunos.

**Dificuldade na implementação:**
Quando alguma operação modifica um nó, vários nós podem ser criados. Se há algum ponteiro apontando para um nó da estrutura, pode ser que este nó não seja o nó da última versão. Para continuar realizando as operações de modificação, estes ponteiros precisam ser atualizados para a última versão daquele nó. Isto inclui, por exemplo, a raiz da estrutura que pode ser modificada e a nova raiz precisa ser utilizada. Ou no caso de uma rotação sobre uma árvore rubro-negra onde dois nós são mantidos e atualizamos ponteiros sobre ambos.
- Dado que estamos implementando persistência parcial, uma forma de resolver este problema é de que cada nó tem um ponteiro para a sua próxima versão, se alguma versão nova deste nó for criada. Uma vez que alguma modificação é feita em algum nó, toda variável que aponta para algum nó deve ser atualizada para a última versão usando estes ponteiros. 
- Uma segunda forma de resolver este problema é que uma quantidade constante de ponteiros para nós podem ser passados para cada operação de atualização. Sempre que um nó for duplicado, este nó deve ser verificado se está na lista de nós mantidos na operação. Caso seja um destes nós, a lista deve ser atualizada com o ponteiro para a última versão deste nó.
