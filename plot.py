import matplotlib.pyplot as plt


def ler_dados(caminho_arquivo, tipo_operacao="Input"):
    valores = []
    with open(caminho_arquivo, 'r') as arquivo:
        for linha in arquivo:
            partes = linha.strip().split(',')
            if partes[1] == tipo_operacao:
                valores.append(int(partes[2]))
    return valores


def plotar_operacoes(arquivo_avl_adicionar, arquivo_avl_remover,
                     arquivo_rb_adicionar, arquivo_rb_remover,
                     arquivo_btree_adicionar_1, arquivo_btree_adicionar_5, arquivo_btree_adicionar_10,
                     arquivo_btree_remover):
    dados_avl_adicionar = ler_dados(arquivo_avl_adicionar, "Input")
    dados_avl_remover = ler_dados(arquivo_avl_remover, "Output")
    dados_rb_adicionar = ler_dados(arquivo_rb_adicionar, "Input")
    dados_rb_remover = ler_dados(arquivo_rb_remover, "Output")
    dados_btree_adicionar_1 = ler_dados(arquivo_btree_adicionar_1, "Input")
    dados_btree_adicionar_5 = ler_dados(arquivo_btree_adicionar_5, "Input")
    dados_btree_adicionar_10 = ler_dados(arquivo_btree_adicionar_10, "Input")
    dados_btree_remover = ler_dados(arquivo_btree_remover, "Output")

    x_avl = list(range(1, len(dados_avl_adicionar) + 1))
    x_rb = list(range(1, len(dados_rb_adicionar) + 1))
    x_btree_1 = list(range(1, len(dados_btree_adicionar_1) + 1))
    x_btree_5 = list(range(1, len(dados_btree_adicionar_5) + 1))
    x_btree_10 = list(range(1, len(dados_btree_adicionar_10) + 1))

    plt.figure(figsize=(10, 6))
    plt.plot(x_avl, dados_avl_adicionar, label='Árvore AVL Adicionar', marker='o')
    plt.plot(x_rb, dados_rb_adicionar, label='Árvore Red-Black Adicionar', marker='x')
    plt.plot(x_btree_1, dados_btree_adicionar_1, label='Árvore B Ordem 1 Adicionar', marker='s')
    plt.plot(x_btree_5, dados_btree_adicionar_5, label='Árvore B Ordem 5 Adicionar', marker='^')
    plt.plot(x_btree_10, dados_btree_adicionar_10, label='Árvore B Ordem 10 Adicionar', marker='d')

    plt.xlabel('Iteração')
    plt.ylabel('Quantidade de Operações (Adicionar)')
    plt.title('Adições (Operações de Adicionar) para Diferentes Estruturas de Árvore')
    plt.legend()
    plt.show()

    plt.figure(figsize=(10, 6))
    plt.plot(x_avl, dados_avl_remover, label='Árvore AVL Remover', marker='o')
    plt.plot(x_rb, dados_rb_remover, label='Árvore Red-Black Remover', marker='x')
    plt.plot(x_btree_1, dados_btree_remover, label='Árvore B Ordem 1 Remover', marker='s')
    plt.plot(x_btree_5, dados_btree_remover, label='Árvore B Ordem 5 Remover', marker='^')
    plt.plot(x_btree_10, dados_btree_remover, label='Árvore B Ordem 10 Remover', marker='d')

    plt.xlabel('Iteração')
    plt.ylabel('Quantidade de Operações (Remover)')
    plt.title('Remoções (Operações de Remover) para Diferentes Estruturas de Árvore')
    plt.legend()
    plt.show()


plotar_operacoes('AVLTreeInput.txt', 'AVLTreeOutput.txt',
                 'RBTreeInput.txt', 'RBTreeOutput.txt',
                 'BTreeInput1.txt', 'BTreeInput5.txt', 'BTreeInput10.txt',
                 'BTreeOutput.txt')
