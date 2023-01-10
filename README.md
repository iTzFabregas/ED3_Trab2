<h1>Estrutura de Dados III - Trabalho 2</h1>

<div align="justify" >
<p>Este trabalho teve como objetivo indexar arquivos de dados usando um índice
árvore-B.</p>
<p>O tamanho da página de disco adotado foi de 73 <i>bytes</i>. A estrutura da árvore-B conta com registro de cabeçalho
e registro de dados. A ordem da árvore-B é 5. Portanto, um nó (página) tem 4 chaves de busca e 5 descendentes.</p>
<p>O programa oferece as seguintes funcionalidades:</p></br>

<li>7) Criar um arquivo de índice árvore-B que indexa a chave de busca definida sobre um
arquivo de dados de entrada já existente. Registros
logicamente removidos presentes no arquivo de dados de entrada não devem ter suas
chaves de busca correspondentes no arquivo de índice. A inserção no arquivo de
índice deve ser feita um-a-um. Ou seja, para cada registro não removido presente no
arquivo de dados, deve ser feita a inserção de sua chave de busca correspondente no
arquivo de índice árvore-B. Considere que não existe repetição nos valores de busca e
que, portanto, essa situação não precisa ser tratada.</br>

<strong>Entrada do programa para a funcionalidade:</br>
7 arquivoDados.bin arquivoIndice.bin</strong>
<hr>

<li>8) Permitir a recuperação dos dados de todos os registros de um arquivo de dados de
entrada, de forma que esses registros satisfaçam um critério de busca determinado pelo
usuário. Qualquer campo pode ser utilizado como forma de busca. Quando o campo usado na busca for a chave de busca,
então a busca deve ser feita usando o índice árvore-B.
Caso contrário, ou seja, quando o campo usado na busca não for a chave de busca,
então a busca deve ser feita diretamente no arquivo binário. Essa funcionalidade pode ser executada N vezes.</br>

<strong>Entrada do programa para a funcionalidade:</br>
8 arquivoDados.bin arquivoIndice.bin N</br>
nomeCampo1 valorCampo1</br>
...</br>
nomeCampoN valorCampoN</strong>
<hr>

<li>9) Permitir a inserção de novos registros em um arquivo de dados de entrada. Depois da inserção do registro no 
arquivo de dados de entrada, deve ser realizada a inserção de sua chave de busca correspondente no arquivo de
índice árvore-B. Essa funcionalidade pode ser executada N vezes.</br>

<strong>Entrada do programa para a funcionalidade:</br>
9 arquivoDados.bin arquivoIndice.bin N</br>
idConecta1 nomePoPs1 nomePais1 siglaPais1 idPoPsConectado1 medidaVelocidade1 velocidade1</br>
...</br>
idConectaN nomePoPsN nomePaisN siglaPaisN idPoPsConectadoN medidaVelocidadeN velocidadeN</strong>
<hr>

<li>10) Permitir a recuperação dos dados de todos os registros armazenados no arquivo de
dados topologiaRede.bin considerando o campo idConecta, juntando-os de forma
apropriada com os dados de topologiaRede.bin considerando o campo
idPoPsConectado. Essa funcionalidade requer a realização de uma autojunção, ou
seja, o mesmo arquivo é usado duas vezes na junção, considerando como condição de
junção topologiaRede1.idConecta = topologiaRede2.idPoPsConectado. Existem
várias formas de se implementar a junção e, nesta funcionalidade, ela deve ser
implementada por meio da junção de loop único. A junção de loop único depende da
existência de índices.</br>
O algoritmo da junção de loop único é definido como segue. Para cada
registro presente no arquivo arquivoA (loop externo), recupere cada registro do
arquivo arquivoB (loop interno) usando o índice e teste se os dois registros
satisfazem à condição de junção Acampo1 = Bcampo1.</br>

<strong>Entrada do programa para a funcionalidade:</br>
10 arquivoDados1.bin arquivoDados2.bin nomeCampoArquivo1, nomeCampoArquivo2, arquivoIndiceDados2.bin</strong>
<hr>

<p>Para compilar os códigos, use o comando <code>make all</code></p>
<p>Para executar o programa, use o comando <code>make run</code></p>
<p>Para testar o programa, pode-se usar os arquivos <strong>binario1.bin</strong> e <strong>indice4.bin</strong></p>
