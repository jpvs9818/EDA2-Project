# EDA2-Trabalho - árvores AVL, rubro-negra e B
![Add](https://github.com/user-attachments/assets/515dcd77-0811-457a-b514-70112b907472)
![Rem](https://github.com/user-attachments/assets/4479ddf5-c73d-4e9e-b0e8-6fe5fa0f3f95)

Metodologia: Em cada um dos códigos foi adicionado uma variavel global “cont” que registra a quantidade de interações mais significativas dentro do código.

Todas as funções possuem um valor de interação minimo

Funções com Loops possuem um cont para representar cada iteração dentro do loop

Funções que receberam valores maiores do que 1 são que possuem loops for or while ou que possuam verificações com if elses.

Gráfico obtido ao iserir 10k valores aleatórios de [1..10000] em cada estrutura e obter o número de interações dentro do código

Cada teste foi realizado 10x para obter um resultado mais preciso

Conclusões:
Os gráficos demonstram que o número de interações em cada teste manteve uma média constante. A árvore AVL apresentou o maior número de interações, comprovando que a árvore Rubro-Negra é mais eficiente em operações de inserção e remoção. Já a árvore B de ordem 10 exibiu um crescimento significativo em relação às outras estruturas.
