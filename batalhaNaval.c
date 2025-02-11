#include <stdio.h>
#include <stdlib.h>

// Tamanho maximo da matriz (x,y)
#define Matriz_X_Max 10
#define Matriz_Y_Max 10

// Dados de posição
typedef struct
{
    int x_start;
    int x_end;
    int y_start;
    int y_end;
} xy_offset;

void ExibirMatriz(int **Matriz);

int **CarregarMatriz();

int xy_offset_ValidPositions(xy_offset *offset);

int SpaceVerification(xy_offset *offset, int **Matriz, int Verticalmente, int Horizontalmente);

void PosicionarNavilVerticalmente(xy_offset *offset,int **Matriz);

void PosicionarNavilHorizontalmente(xy_offset *offset, int **Matriz);

void xy_offset_Editor(xy_offset *offset, int x_start, int x_end, int y_start, int y_end);

void Habilidade(int HabilidadeSpecial, xy_offset *offset, int **Matriz);

int Menu(xy_offset *offset, int **Matriz,int **MatrizDeHabilidade,int Finalizar);

int main()
{

    int FinalizarJogo = 0;
    int **Matriz = CarregarMatriz();             // --> aonde os navil se encontra
    int **MatrizDeHabilidade = CarregarMatriz(); // --> Aonde as habilidades se encontra
    int **Matriz_Final = CarregarMatriz();       // --> resultado final (navil & habilidades)

    // --> Usado tanto pra posicionar o barco quanto as habilidades.
    xy_offset *barco_place_position = malloc(sizeof(xy_offset));
    barco_place_position->x_start = 0;
    barco_place_position->x_end = 0;
    barco_place_position->y_end = 0;
    barco_place_position->y_start = 0;

    // Preencher espaços com "0" (indicativo de que não tem nenhum navio ocupando o espaço)
    for (int i = 0; i < Matriz_X_Max; i++)
    {
        for (int j = 0; j < Matriz_Y_Max; j++)
        {
            Matriz[i][j] = 0;
            MatrizDeHabilidade[i][j] = 0;
            Matriz_Final[i][j] = 0;
        }
    }

    do
    {
        FinalizarJogo = Menu(barco_place_position,Matriz,MatrizDeHabilidade,FinalizarJogo);
    } while (FinalizarJogo < 1);

    // juntar a matriz de habilidade com a do navil.
    for (int i = 0; i < Matriz_X_Max; i++)
    {
        for (int j = 0; j < Matriz_Y_Max; j++)
        {
            if (MatrizDeHabilidade[i][j] > 0) {
                Matriz_Final[i][j] = 1;  
            }

            if (Matriz[i][j] > 0 && MatrizDeHabilidade[i][j] <= 0) {
                Matriz_Final[i][j] = 3;  
            }
        }
    }

    ExibirMatriz(Matriz_Final);
    
    return 0;
}

int Menu(xy_offset *offset, int **Matriz,int **MatrizDeHabilidade,int Finalizar){
    int selected_menu_option = 0;
    // Menu do jogo:
    printf("\n|======================|\n");
    printf("|1. Adicionar um navil \n");
    printf("|2. Usar uma habilidade \n");
    printf("|3. Finalizar jogo \n");
    printf("|======================|\n");

    scanf("%d",&selected_menu_option);

    switch (selected_menu_option)
    {
    case 1:
    printf("\n||como quer posicionar o navil?\n");
    printf("||1. Vertical\n");
    printf("||2. Horizontal\n");

    int position_selected = 0;
    scanf("%d",&position_selected);

    ExibirMatriz(Matriz);
    printf("\n");

    (position_selected == 2) ? (PosicionarNavilHorizontalmente(offset,Matriz)) : (position_selected);
    (position_selected == 1) ? (PosicionarNavilVerticalmente(offset,Matriz)) : (position_selected);
    (position_selected <= 0 || position_selected > 2) ? (printf("\n||Opção invalida!")) : (printf("\n||Opção invalida!"));

    printf("\n||Resultado: \n");
    ExibirMatriz(Matriz);

    break;

    case 2:
    printf("\n||Qual habilidade vai usar?\n");
    printf("||1. Cone\n");
    printf("||2. Octaedro\n");
    printf("||2. Cruz\n");

    position_selected = 0;
    scanf("%d",&position_selected);

    int x,y;

    // --> pedir as posições para o jogador da onde vai usar
    ExibirMatriz(MatrizDeHabilidade);
    printf("\n \n||Aonde pretende usar a habilidade? (x = linha, y = coluna)\n (OBS: 0 = *casa livre*) \n");
    printf("\n||Y (OBS: escolha um numero que possua os proximos 2 numeros abaixo dele livre): ");
    scanf("%d",&y);
    printf("\n||X: ");
    scanf("%d",&x);

    xy_offset_Editor(offset,(x-1),5,(y-1),5);
    printf("\n%d %d\n",y,x);

    Habilidade(position_selected,offset,MatrizDeHabilidade);

    break;

    case 3:
    printf("\n \n||Resultado final:");
    return 1;
    break;
    
    default:
    printf("\n||Essa opção é invalida!\n");
    break;
    }

    return 0;
}

void ExibirMatriz(int **Matriz)
{
    printf("\n");
    printf("  Y\n  ");
    for (int r = 0; r < 2; r++)
    {
        if (r < 1)
        {
            for (int i = 0; i < Matriz_X_Max; i++)
            {
                printf("%d ", (i + 1));

                ((i + 1) >= Matriz_X_Max) ? (printf("\n  ")) : (i);
            }
        }
        else
        {
            for (int i = 0; i < Matriz_X_Max; i++)
            {
                printf("| ");
            }
        }
    }
    printf("\nX");
    for (int i = 0; i < Matriz_X_Max; i++)
    {
        printf(" ");
        for (int j = 0; j < Matriz_Y_Max; j++)
        {
            printf("%d ", Matriz[i][j]);
        }
        printf("-%d\n ", (i + 1));
    }
}

int xy_offset_ValidPositions(xy_offset *offset)
{
    int ValidValues = 0; // --> (1) = Invalid Positions Values.

    (offset->x_end > Matriz_X_Max - 1 || offset->x_start < 0) ? (ValidValues = 1) : (ValidValues);
    (ValidValues == 0 && (offset->y_end > Matriz_Y_Max - 1 || offset->y_start < 0)) ? (ValidValues = 1) : (ValidValues);
    return ValidValues;
}

void xy_offset_Editor(xy_offset *offset, int x_start, int x_end, int y_start, int y_end)
{
    offset->x_start = x_start;
    offset->x_end = x_end;
    offset->y_start = y_start;
    offset->y_end = y_end;
}

void Habilidade(int HabilidadeSpecial, xy_offset *offset, int **Matriz)
{
    int ValidOffset = xy_offset_ValidPositions(offset);

    if (ValidOffset > 0)
    {
        return;
    }

    xy_offset *offsetCheck = malloc(sizeof(xy_offset));
    offsetCheck->x_start = offset->x_start;
    offsetCheck->x_end = offset->x_end;
    offsetCheck->y_start = offset->y_start;
    offsetCheck->y_end = offset->y_end;

    printf("\n[Habilidade]: %d\n",HabilidadeSpecial);

    switch (HabilidadeSpecial)
    {
    case 1: // Habilidade Cone:
    /*
    0 0 1 0 0
    0 1 1 1 0
    1 1 1 1 1
    0 0 0 0 0

    verifique se os espaços estão livres:
    y_start , y_end

    x_start , (x_start + 1) , (x_Start - 1)
    */

    int y_valid_positions = SpaceVerification(offset,Matriz,1,0);

    if (y_valid_positions > 0)
    {
        return;
    }

    int x_InvalidSpace = 0;
    int x_expansion = 0;

    if (Matriz[offset->y_start][offset->x_start] > 0){
        return;
    }

    for (int i = 0; i < 3 ; i++){
        offsetCheck->x_start = offset->x_start - i;
        offsetCheck->x_end = offset->x_start + i;
        offsetCheck->y_start = offset->y_start + i;

        if (xy_offset_ValidPositions(offsetCheck) > 0){
            x_InvalidSpace = 1;
            return;
        }
    }

    for (int i = 0; i < 3 ; i++){
        Matriz[offset->y_start + i][offset->x_start] = 1;
        if (i > 0){
            for (int x = 1; x < (i + 1); x++){
                Matriz[offset->y_start + i][offset->x_start + x] = 1;
                Matriz[offset->y_start + i][offset->x_start - x] = 1;
            }
        }
    }

    ExibirMatriz(Matriz);

    break;

    case 2:

    // Habilidade Cone:
    /*
    0 0 1 0 0
    0 1 1 1 0
    0 0 1 0 0
    0 0 0 0 0

    verifique se os espaços estão livres:
    y_start , y_end

    x_start , (x_start + 1) , (x_Start - 1)
    */

    y_valid_positions = SpaceVerification(offset,Matriz,1,0);

    if (y_valid_positions > 0)
    {
        return;
    }

    x_InvalidSpace = 0;
    x_expansion = 0;

    if (Matriz[offset->y_start][offset->x_start] > 0){
        return;
    }

    for (int i = 0; i < 3 ; i++){
        offsetCheck->x_start = offset->x_start - x_expansion;
        offsetCheck->x_end = offset->x_start + x_expansion;
        offsetCheck->y_start = offset->y_start + i;

        if (xy_offset_ValidPositions(offsetCheck) > 0){
            x_InvalidSpace = 1;
            return;
        }

        (x_expansion >= 1) ? (x_expansion--) : (x_expansion++);
    }

    x_expansion = 0;

    for (int i = 0; i < 3 ; i++){
        Matriz[offset->y_start + i][offset->x_start] = 1;
        if (i > 0){
            Matriz[offset->y_start + i][offset->x_start + x_expansion] = 1;
            Matriz[offset->y_start + i][offset->x_start - x_expansion] = 1;
        }

        (x_expansion >= 1) ? (x_expansion--) : (x_expansion++);
    }

    ExibirMatriz(Matriz);

    break;

    case 3:

    // Habilidade Cone:
    /*
    0 0 1 0 0
    1 1 1 1 1
    0 0 1 0 0
    0 0 0 0 0

    verifique se os espaços estão livres:
    y_start , y_end

    x_start , (x_start + 1) , (x_Start - 1)
    */

    y_valid_positions = SpaceVerification(offset,Matriz,1,0);

    if (y_valid_positions > 0)
    {
        return;
    }

    x_InvalidSpace = 0;
    x_expansion = 0;

    if (Matriz[offset->y_start][offset->x_start] > 0){
        return;
    }

    for (int i = 0; i < 3 ; i++){
        offsetCheck->x_start = offset->x_start - x_expansion;
        offsetCheck->x_end = offset->x_start + x_expansion;
        offsetCheck->y_start = offset->y_start + i;

        if (xy_offset_ValidPositions(offsetCheck) > 0){
            x_InvalidSpace = 1;
            return;
        }

        (x_expansion >= 1) ? (x_expansion=0) : (x_expansion+=2);
    }

    x_expansion = 0;

    for (int i = 0; i < 3 ; i++){
        Matriz[offset->y_start + i][offset->x_start] = 1;
        if (x_expansion > 0){
            for (int x = 1; x < (x_expansion + 1); x++){
                Matriz[offset->y_start + i][offset->x_start + x] = 1;
                Matriz[offset->y_start + i][offset->x_start - x] = 1;
            }
        }

        (x_expansion >= 1) ? (x_expansion=0) : (x_expansion+=2);
    }

    ExibirMatriz(Matriz);

    break;

    default:
    printf("\nOpção invalida!\n");

    break;
    }
}

void PosicionarNavilHorizontalmente(xy_offset *offset, int **Matriz)
{
    int valor_x_1, valor_x_2, valor_y;
    int x_start, x_end, y_start;
    printf("\n*Digite as posições que pretende possicionar o navio: \n");
    // --> Obtenha as posições y (começo e fim);
    printf("*Digite uma posição da coluna X: ");
    scanf("%d", &valor_x_1);
    printf("\n*Digite a segunda posição da coluna X (Diferente da primeira): ");
    scanf("%d", &valor_x_2);

    // --> Obtenha a posição do x:
    printf("*Digite uma posição da coluna Y: ");
    scanf("%d", &valor_y);

    // --> Fix Values: (y_start - sempre o menor valor, y_end - sempre o maior valor)
    (valor_x_1 > valor_x_2) ? (x_start = valor_x_2, x_end = valor_x_1) : (x_start = valor_x_1, x_end = valor_x_2);
    y_start = valor_y - 1;
    x_start -= 1;
    x_end -= 1;

    // --> Verificação de valores invalidos

    xy_offset_Editor(offset, x_start, x_end, y_start, 0);

    if (xy_offset_ValidPositions(offset) > 0){
        return;
    }

    if (valor_x_1 == valor_x_2)
    {
        printf("\n*Valores repitidos: %d %d", valor_x_1, valor_x_2);
        return;
    }

    int AlreadyTakenSpace = SpaceVerification(offset, Matriz, 0, 1);

    if (AlreadyTakenSpace > 0)
    {
        return;
    }

    int ValorInicial = x_start;
    int TotalDeCasas = 0;    // Quantas vezes vai se repetir.
    int CasaAtual = x_start; // Valor usado só pra identificar quantas vezes tera que repetir pra chegar no valor final.

    while (CasaAtual <= x_end)
    {
        CasaAtual++;
        TotalDeCasas++;
    }

    printf("\n%d total de repetições", TotalDeCasas);

    for (int i = 0; i < TotalDeCasas; i++)
    {
        if (Matriz[y_start][ValorInicial] > 0)
        {
            continue;
        }

        Matriz[y_start][ValorInicial] = 3;
        ValorInicial++;
    }
    return;
}

void PosicionarNavilVerticalmente(xy_offset *offset,int **Matriz)
{
    int valor_y_1, valor_y_2, valor_x;
    int y_start, y_end, x_start;
    printf("\n*Digite as posições que pretende possicionar o navio: \n");
    // --> Obtenha as posições y (começo e fim);
    printf("*Digite uma posição da coluna Y: ");
    scanf("%d", &valor_y_1);
    printf("\n*Digite a segunda posição da coluna Y (Diferente da primeira): ");
    scanf("%d", &valor_y_2);

    // --> Obtenha a posição do x:
    printf("*Digite uma posição da coluna X: ");
    scanf("%d", &valor_x);

    // --> Fix Values: (y_start - sempre o menor valor, y_end - sempre o maior valor)
    (valor_y_1 > valor_y_2) ? (y_start = valor_y_2, y_end = valor_y_1) : (y_start = valor_y_1, y_end = valor_y_2);
    x_start = valor_x - 1;
    y_start -= 1;
    y_end -= 1;

    xy_offset *Navio_Position = malloc(sizeof(xy_offset));
    xy_offset_Editor(offset, x_start, 0, y_start, y_end);

    if (xy_offset_ValidPositions(offset) > 0){
        return;
    }

    if (valor_y_1 == valor_y_2)
    {
        printf("\n*Valores repitidos: %d %d", valor_y_1, valor_y_2);
        return;
    }

    int AlreadyTakenSpace = SpaceVerification(offset, Matriz, 1, 0);

    if (AlreadyTakenSpace > 0)
    {
        return;
    }

    int ValorInicial = y_start;
    int TotalDeCasas = 0;    // Quantas vezes vai se repetir.
    int CasaAtual = y_start; // Valor usado só pra identificar quantas vezes tera que repetir pra chegar no valor final.

    while (CasaAtual <= y_end)
    {
        CasaAtual++;
        TotalDeCasas++;
    }

    printf("\n%d total de repetições", TotalDeCasas);

    for (int i = 0; i < TotalDeCasas; i++)
    {
        if (Matriz[ValorInicial][x_start] > 0)
        {
            continue;
        }

        Matriz[ValorInicial][x_start] = 3;
        ValorInicial++;
    }
    return;
}

int SpaceVerification(xy_offset *offset, int **Matriz, int Verticalmente, int Horizontalmente)
{
    if (Verticalmente > 0 && Horizontalmente > 0)
    {
        // Não é possível verificar os dois lados com os dois lados
        return 1;
    }

    int UniqueVerification = 0;
    (offset->x_start == offset->x_end && offset->y_start == offset->y_end) ? (UniqueVerification = 1) : (UniqueVerification);

    if (UniqueVerification > 0)
    {
        int SpaceAlreadyTaken = 0; // se o valor for (1) é porque sim, o espaço já esta ocupado.

        (Matriz[offset->y_start][offset->x_start] > 0) ? (SpaceAlreadyTaken = 1) : (SpaceAlreadyTaken);

        return SpaceAlreadyTaken;
    }

    if (Horizontalmente > 0)
    {
        int SpaceAlreadyTaken = 0; // se o valor for (1) é porque sim, os espaços já estão ocupados.

        int x_inicio, x_fim;
        /*
        Informações que serão usadas pra saber a posição:

        offset.x_start
        offset.x_end
        offset.y_start

        */

        (offset->x_start > offset->x_end) ? (x_inicio = offset->x_end, x_fim = offset->x_start) : (x_inicio = offset->x_start, x_fim = offset->x_end);
        x_inicio -= 1;
        x_fim -= 1;

        int CasaAtual = x_inicio;
        int x_e_found = x_inicio;

        int Total = 0;

        while (x_e_found <= x_fim)
        {
            x_e_found++;
            Total++;
        }

        if (Matriz[offset->y_start - 1][x_inicio] > 0)
        {
            SpaceAlreadyTaken = 1;
        }

        for (int i = 0; i < (Total); i++)
        {
            if (SpaceAlreadyTaken > 0)
            {
                break;
            }

            if (Matriz[offset->y_start - 1][CasaAtual] > 0)
            {
                SpaceAlreadyTaken = 1;
            }

            CasaAtual++;
        }

        return SpaceAlreadyTaken;
    }

    if (Verticalmente > 0)
    {
        int SpaceAlreadyTaken = 0; // se o valor for (1) é porque sim, os espaços já estão ocupados.

        int y_inicio, y_fim;
        /*
        Informações que serão usadas pra saber a posição:

        offset.y_start
        offset.y_end
        offset.x_start

        */

        (offset->y_start > offset->y_end) ? (y_inicio = offset->y_end, y_fim = offset->y_start) : (y_inicio = offset->y_start, y_fim = offset->y_end);
        y_inicio -= 1;
        y_fim -= 1;

        int CasaAtual = y_inicio;
        int y_e_found = y_inicio;

        int Total = 0;

        while (y_e_found <= y_fim)
        {
            y_e_found++;
            Total++;
        }

        if (Matriz[y_inicio][offset->x_start - 1] > 0)
        {
            SpaceAlreadyTaken = 1;
        }

        for (int i = 0; i < (Total); i++)
        {
            if (SpaceAlreadyTaken > 0)
            {
                break;
            }

            if (Matriz[CasaAtual][offset->x_start - 1] > 0)
            {
                SpaceAlreadyTaken = 1;
            }

            CasaAtual++;
        }

        return SpaceAlreadyTaken;
    }
}

int **CarregarMatriz()
{
    int **Matriz_ = malloc(Matriz_Y_Max * sizeof(int));

    for (int i = 0; i < Matriz_X_Max; i++)
    {
        Matriz_[i] = malloc(Matriz_X_Max * sizeof(int));
    }

    return Matriz_;
}
