# Trabalho 1 - INE5645

#### Bernardo De Marco Gonçalves - 22102557

## _Design_ da aplicação 

A aplicação é composta por N sensores (valor parametrizável via _input_). Cada sensor é uma _thread_ que executa um _loop_ inifinito, produzindo valores aleatórios e os inserindo no _buffer_ do produtor/consumidor.

O _core_ da aplicação concentra-se na central de controle (_orchestrator_). Esse componente é uma única _thread_ que gerencia os atuadores do veículo, através de um _thread pool_ de `4` _threads_ e uma tabela de controle. Além disso, o _orchestrator_ é o o responsável por consumir os dados produzidos pelos sensores do veículo.

Sempre que o _orchestrator_ consumir um valor do _buffer_, ele envia o valor juntamente com uma _task_ a ser executada para uma _thread_ do _thread pool_. Cada _task_ é responsável por determinar o atuador e o nível de atividade, atualizar o registro do atuador e gerar um _log_ no terminal. 

A atualização e a geração dos _logs_ são executados em paralelo (_fork-join_). Caso ocorra algum erro em alguma dessas subtarefas, a _task_ é responsável por gerar uma saída informando o erro ocorrido. 

Para atualização do atuador, o seu nível de atividade na tabela de atuadores é atualizado, e permanece inalterado por dois ou três segundos. Para não definir a tabela inteira como uma única zona de exclusão mútua, ela foi dividida em seções críticas com uma granularidade pré-determinada de `20`. Ou seja, quando um atuador tiver seu nível de atividade atualizado apenas a sua seção será travada, e não a tabela inteira. 

Concomitantemente à atualização da tabela, o _log_ é gerado. Sempre que um _log_ é impresso, ele será mantido no console por um segundo. Por fim, a _task_ do _thread pool_, ao sincronizar o _fork-join_, checa se houve algum erro em alguma tarefa (`20%` de chance de erro em cada uma). Caso algum erro tenha ocorrido, um _log_ de erro é gerado e impresso no terminal.

## Tecnologias adotadas

- Linguagem de programação C
- POSIX _library_
- [C-Thread-Pool](https://github.com/Pithikos/C-Thread-Pool)

A biblioteca [C-Thread-Pool](https://github.com/Pithikos/C-Thread-Pool) foi utilizada para o uso de _thread pools_. Ela possui uma API clara e de fácil utilização. Além disso, seu código-fonte foi verificado e foi certificado que ela utiliza uma _queue_ como _buffer_ de _tasks_ do _pool_. Com isso, foi possível utilizá-la para cumprir requisitos da aplicação.

## Instruções de _setup_

Foi utilizado o `Makefile` para facilitar a compilação da aplicação. O arquivo compilado é o arquivo `app` localizado na pasta `build`.

- Compilar:
    ```bash
    make compile
    ```
- Rodar:
    ```bash
    make run
    ```
- Limpar:
    ```bash
    make clean
    ```