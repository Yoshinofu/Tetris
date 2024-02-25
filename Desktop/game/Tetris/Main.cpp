#include "DxLib.h"
#include <math.h>
#include <cstdlib>

#define PI 3.1415926f

int Key[256];
int function_status = 0;
int Clolr_White = GetColor(255, 255, 255);
int Clolr_Black = GetColor(0, 0, 0);
int Clolr_Red = GetColor(255, 0, 0);
int Clolr_Yellow = GetColor(255, 255, 0);
int Clolr_Lime = GetColor(0, 255, 0);
int Clolr_Aqua = GetColor(0, 255, 255);
int Clolr_Purple = GetColor(255, 0, 255);
int Clolr_Blue = GetColor(0, 0, 255);
int Clolr_Orange = GetColor(255, 128, 0);
int Clolr_Gray = GetColor(128, 128, 128);

typedef struct {
    int x, y;       // ���W�i�[�p�ϐ�
    char name[128]; // ���ږ��i�[�p�ϐ�
} MenuElement_t;

typedef struct {
    int x, y;
    int Num_Spin;
} MinoElement_t;

static const int Block_Size = 20;

static const int Field_Width = 12;
static const int Field_Height = 26;

static const int Reference_Point_X = 220;
static const int Reference_Point_Y = -60;

static const int N_Reference_Point_X = 500;
static const int N_Reference_Point_Y = 30;

static const int F_Reference_Point_X = 30;
static const int F_Reference_Point_Y = 150;

int count = 0;

int Field_list[Field_Height][Field_Width];
int Drop_Mino_list[4][4];
int Fold_Mino_list[4][4];

int Next_Num_Mino_list[5];
int Next_Mino_list[5][4][4];

int Fold_flag = 0;
int Fold_Mino = 0;

// �L�[�̓��͏�Ԃ��X�V����
int gpUpdateKey() {
    char tmpKey[256]; // ���݂̃L�[�̓��͏�Ԃ��i�[����
    GetHitKeyStateAll(tmpKey); // �S�ẴL�[�̓��͏�Ԃ𓾂�
    for (int i = 0; i < 256; i++) {
        if (tmpKey[i] != 0) { // i�Ԃ̃L�[�R�[�h�ɑΉ�����L�[��������Ă�����
            Key[i]++;     // ���Z
        }
        else {              // ������Ă��Ȃ����
            Key[i] = 0;   // 0�ɂ���
        }
    }
    return 0;
}

void Opning(int* SelectNum, MenuElement_t MenuElement[]) {

    if (Key[KEY_INPUT_DOWN] == 1) { 

        *SelectNum = (*SelectNum + 1) % 5; 
    }

    if (Key[KEY_INPUT_UP] == 1) { 

        *SelectNum = (*SelectNum + 4) % 5; 
    }

    if (Key[KEY_INPUT_DOWN] == 1 || Key[KEY_INPUT_UP] == 1) { 
        for (int i = 0; i < 5; i++) {             
            if (i == *SelectNum) {        
                MenuElement[i].x = 80;
            }
            else {                       
                MenuElement[i].x = 100;
            }
        }
    }

    //�Z���N�g
    if (Key[KEY_INPUT_SPACE] == 1 && *SelectNum == 0) {
        function_status = 10;
    }

    // �`��t�F�[�Y

    for (int i = 0; i < 5; i++) { // ���j���[���ڂ�`��
        DrawFormatString(MenuElement[i].x, MenuElement[i].y, GetColor(255, 255, 255), MenuElement[i].name);
    }

}

int Ending() {
    return 0;
}


void Block(int x, int y, int Clolr) { //�����̓}�X���W
    int d_x = Reference_Point_X + x * ( Block_Size + 1), d_y = Reference_Point_Y + y * (Block_Size + 1);
    for (int i = 0; i < Block_Size; i++) {
        for (int j = 0; j < Block_Size; j++) {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
            if ( i==0 || i==Block_Size-1 || j==0 || j==Block_Size-1) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            DrawPixel(d_x + i, d_y + j, Clolr);
        }
    }
}


void N_Block(int x, int y, int Clolr) { //�����̓}�X���W
    int d_x = N_Reference_Point_X + x * ( Block_Size + 1 ), d_y = N_Reference_Point_Y + y * ( Block_Size + 1 );
    for (int i = 0; i < Block_Size; i++) {
        for (int j = 0; j < Block_Size; j++) {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
            if (i == 0 || i == Block_Size - 1 || j == 0 || j == Block_Size - 1) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            DrawPixel(d_x + i, d_y + j, Clolr);
        }
    }
}

void F_Block(int x, int y, int Clolr) { //�����̓}�X���W
    int d_x = F_Reference_Point_X + x * (Block_Size + 1), d_y = F_Reference_Point_Y + y * (Block_Size + 1);
    for (int i = 0; i < Block_Size; i++) {
        for (int j = 0; j < Block_Size; j++) {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
            if (i == 0 || i == Block_Size - 1 || j == 0 || j == Block_Size - 1) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            DrawPixel(d_x + i, d_y + j, Clolr);
        }
    }
}

void G_Block(int x, int y, int Clolr) { //�����̓}�X���W
    int d_x = Reference_Point_X + x * (Block_Size + 1), d_y = Reference_Point_Y + y * (Block_Size + 1);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 70);
    for (int i = 0; i < Block_Size; i++) {
        for (int j = 0; j < Block_Size; j++) {
            DrawPixel(d_x + i, d_y + j, Clolr);
        }
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


void Flame() { //�t���[������
    for (int i = 0; i < Field_Height; i++) {
        Field_list[i][0] = 30;
        Field_list[i][Field_Width - 1] = 30;
    }
    for (int i = 0; i < Field_Width; i++) {
        Field_list[Field_Height - 1][i] = 30;
    }
}

void T_Block(int Num_Spin, int Mino_list[4][4]) {
    Num_Spin = Num_Spin % 4;


    switch (Num_Spin) {
    case 0:
        for (int i = 0; i < 4; i++) {
            Mino_list[1][2] = 1;
            Mino_list[2][1] = 1;
            Mino_list[2][2] = 1;
            Mino_list[2][3] = 1;
        }
        break;

    case 1:
        for (int i = 0; i < 4; i++) {
            Mino_list[1][2] = 1;
            Mino_list[2][2] = 1;
            Mino_list[2][3] = 1;
            Mino_list[3][2] = 1;
        }
        break;

    case 2:

        for (int i = 0; i < 4; i++) {

            Mino_list[1][1] = 1;
            Mino_list[1][2] = 1;
            Mino_list[1][3] = 1;
            Mino_list[2][2] = 1;
        }
        break;

    case 3:

        for (int i = 0; i < 4; i++) {
            Mino_list[1][2] = 1;
            Mino_list[2][1] = 1;
            Mino_list[2][2] = 1;
            Mino_list[3][2] = 1;
        }
        break;

    default:
        break;
    }
}

void O_Block(int Mino_list[4][4]) {
    for (int i = 0; i < 4; i++) {
        Mino_list[1][1] = 2;
        Mino_list[1][2] = 2;
        Mino_list[2][1] = 2;
        Mino_list[2][2] = 2;
    }
}


void I_Block(int Num_Spin, int Mino_list[4][4]) {
    Num_Spin = Num_Spin % 2;


    switch (Num_Spin) {
    case 0:
        for (int i = 0; i < 4; i++) {
            Mino_list[0][1] = 3;
            Mino_list[1][1] = 3;
            Mino_list[2][1] = 3;
            Mino_list[3][1] = 3;
        }

        break;
    case 1:

        for (int i = 0; i < 4; i++) {
            Mino_list[1][0] = 3;
            Mino_list[1][1] = 3;
            Mino_list[1][2] = 3;
            Mino_list[1][3] = 3;
        }
        break;

    default:
        break;
    }
}
void S_Block(int Num_Spin, int Mino_list[4][4]) {
    Num_Spin = Num_Spin % 2;


    switch (Num_Spin) {
    case 0:
        for (int i = 0; i < 4; i++) {
            Mino_list[1][3] = 4;
            Mino_list[1][2] = 4;
            Mino_list[2][2] = 4;
            Mino_list[2][1] = 4;
        }

        break;
    case 1:

        for (int i = 0; i < 4; i++) {
            Mino_list[1][1] = 4;
            Mino_list[2][1] = 4;
            Mino_list[2][2] = 4;
            Mino_list[3][2] = 4;
        }

        break;

    default:
        break;
    }
}

void Z_Block(int Num_Spin, int Mino_list[4][4]) {
    Num_Spin = Num_Spin % 2;


    switch (Num_Spin) {
    case 0:
        for (int i = 0; i < 4; i++) {
            Mino_list[1][1] = 5;
            Mino_list[1][2] = 5;
            Mino_list[2][2] = 5;
            Mino_list[2][3] = 5;
        }

        break;
    case 1:

        for (int i = 0; i < 4; i++) {
            Mino_list[0][2] = 5;
            Mino_list[1][2] = 5;
            Mino_list[1][1] = 5;
            Mino_list[2][1] = 5;
        }

        break;

    default:
        break;
    }
}

void L_Block(int Num_Spin, int Mino_list[4][4]) {
    Num_Spin = Num_Spin % 4;


    switch (Num_Spin) {
    case 0:
        for (int i = 0; i < 4; i++) {
            Mino_list[1][1] = 6;
            Mino_list[2][1] = 6;
            Mino_list[3][1] = 6;
            Mino_list[3][2] = 6;
        }

        break;
    case 1:

        for (int i = 0; i < 4; i++) {
            Mino_list[2][1] = 6;
            Mino_list[2][2] = 6;
            Mino_list[2][3] = 6;
            Mino_list[3][1] = 6;
        }

        break;
    case 2:

        for (int i = 0; i < 4; i++) {
            Mino_list[1][1] = 6;
            Mino_list[1][2] = 6;
            Mino_list[2][2] = 6;
            Mino_list[3][2] = 6;
        }

        break;
    case 3:

        for (int i = 0; i < 4; i++) {
            Mino_list[1][3] = 6;
            Mino_list[2][1] = 6;
            Mino_list[2][2] = 6;
            Mino_list[2][3] = 6;
        }
        break;

    default:
        break;
    }
}
void J_Block(int Num_Spin, int Mino_list[4][4]) {
    Num_Spin = Num_Spin % 4;


    switch (Num_Spin) {
    case 0:
        for (int i = 0; i < 4; i++) {
            Mino_list[1][2] = 7;
            Mino_list[2][2] = 7;
            Mino_list[3][2] = 7;
            Mino_list[3][1] = 7;
        }

        break;
    case 1:

        for (int i = 0; i < 4; i++) {
            Mino_list[1][1] = 7;
            Mino_list[2][1] = 7;
            Mino_list[2][2] = 7;
            Mino_list[2][3] = 7;
        }

        break;
    case 2:

        for (int i = 0; i < 4; i++) {
            Mino_list[1][1] = 7;
            Mino_list[1][2] = 7;
            Mino_list[2][1] = 7;
            Mino_list[3][1] = 7;
        }

        break;
    case 3:

        for (int i = 0; i < 4; i++) {
            Mino_list[1][1] = 7;
            Mino_list[1][2] = 7;
            Mino_list[1][3] = 7;
            Mino_list[2][3] = 7;
        }
        break;

    default:
        break;
    }
}

bool Is_Below(int x, int y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (Field_list[y + i + 1][x + j] * Drop_Mino_list[i][j] >= 10 ) {
                return true;
            }
        }
    }
    return false;
}

void Drop(int* x, int* y) { //�P�t���[�����ƂɃu���b�N����

    if ( ! Is_Below(*x, *y)) {
        if (Key[KEY_INPUT_DOWN] >= 1 && count % 15 == 0) {
            *y += 1;
        }

        else if (count % 60 == 0) {
            *y += 1;
        }
    }
}

bool Is_Right(int x, int y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (Field_list[y + i][x + j + 1] * Drop_Mino_list[i][j] >= 10) return true;
        }
    }
    return false;
}

bool Is_Left(int x, int y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (Field_list[y + i][x + j - 1] * Drop_Mino_list[i][j] >= 10) return true;
        }
    }
    return false;
}

void Key_LR(int* x, int* y, int* bfore_key) { //�L�[�{�[�h���͂ɂ����W���f
    if (Key[KEY_INPUT_RIGHT] >= 1 && *bfore_key == 0 && Is_Right(*x, *y) == 0) {
        *bfore_key = 1;
        *x += 1;
    }

    if (Key[KEY_INPUT_LEFT] >= 1 && *bfore_key == 0 && Is_Left(*x, *y) == 0) {
        *bfore_key = 1;
        *x -= 1;
    }

    if (Key[KEY_INPUT_UP] == 0 && Key[KEY_INPUT_RIGHT] == 0 && Key[KEY_INPUT_LEFT] == 0) *bfore_key = 0;
}

void Key_UP(int* x, int* y, int* bfore_key) {
    if (Key[KEY_INPUT_UP] >= 1 && *bfore_key == 0) {
        *bfore_key = 1;
        int i = *y;
        while (Is_Below(*x, i) == 0) {
            i++;
        }
        *y = i;
    }

    if (Key[KEY_INPUT_UP] == 0 && Key[KEY_INPUT_RIGHT] == 0 && Key[KEY_INPUT_LEFT] == 0) *bfore_key = 0;
}

void Key_Spin(int* Num_Spin, int* bfore_space) {
    if (Key[KEY_INPUT_SPACE] >= 1 && *bfore_space == 0) {
        *Num_Spin += 1;
        *bfore_space = 1;
    }
    if (Key[KEY_INPUT_SPACE] == 0) *bfore_space = 0;
}

void Key_Fold(int *Num_Spin) {
    if (Key[KEY_INPUT_RSHIFT] == 1 && Fold_flag == 0) {
        Fold_flag = 1;
        *Num_Spin = 0;

        if (Fold_Mino == 0) {
            Fold_Mino = Next_Num_Mino_list[0];

            for (int i = 0; i < 4; i++) {
                Next_Num_Mino_list[i] = Next_Num_Mino_list[i + 1];
            }

            Next_Num_Mino_list[4] = GetRand(6) + 1;
        }

        else {
            int t;
            t = Fold_Mino;
            Fold_Mino = Next_Num_Mino_list[0];
            Next_Num_Mino_list[0] = t;
        }
        
    }
}

void The_Bottom_y(int x, int y, int *bottom_y) {
    while (!Is_Below(x, y)) {
        y += 1;
    }
    *bottom_y = y;
}


void Drop_Line(int y) {
    for (int j = y; j > 0; j--) {
        for (int i = 1; i < Field_Width - 1; i++) {
            Field_list[j][i] = Field_list[j - 1][i];
        }
    }
}

void Clear_Line() {
    for (int i = 0; i < Field_Height - 1; i++) {
        int j = 1;
        while (Field_list[i][j] >= 10) {
            if (j >= 10) {
                for (int k = 1; k < Field_Width - 1; k++) {
                    Field_list[i][k] = 0;
                }
                Drop_Line(i);
                break;
            }
            j += 1;
        }
    }
}


int Dec_Clolr(int Num) {
    switch (Num) {
    case 1:
        return Clolr_Purple;
    case 2:
        return Clolr_Yellow;
    case 3:
        return Clolr_Aqua;
    case 4:
        return Clolr_Lime;
    case 5:
        return Clolr_Red;
    case 6:
        return Clolr_Orange;
    case 7:
        return Clolr_Blue;
    default:
        return 0;
        break;
    }
}

void Dec_Block(int Num_Spin, int Num_Block, int Mino_list[4][4]) {
    switch (Num_Block) {
    case 1:
        T_Block(Num_Spin, Mino_list);
        break;
    case 2:
        O_Block(Mino_list);
        break;
    case 3:
        I_Block(Num_Spin, Mino_list);
        break;
    case 4:
        S_Block(Num_Spin, Mino_list);
        break;
    case 5:
        Z_Block(Num_Spin, Mino_list);
        break;
    case 6:
        L_Block(Num_Spin, Mino_list);
        break;
    case 7:
        J_Block(Num_Spin, Mino_list);
        break;
    default:
        break;
    }
}

void Reset_Mino_List(int Mino_list[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            Mino_list[i][j] = 0;
        }
    }
}

void Draw_Field() {
    for (int i = 0; i < Field_Height; i++) {
        for (int j = 0; j < Field_Width; j++) {

            if (Field_list[i][j] == 30) {
                Block(j, i, Clolr_White);
            }

            else if (Field_list[i][j] >= 10) {
                Block(j, i, Dec_Clolr(Field_list[i][j] % 10));
            }

            else if (Field_list[i][j] >= 1) {
                Block(j, i, Dec_Clolr(Field_list[i][j]));
                Field_list[i][j] = 0;
            }

            else if (Field_list[i][j] < 0) {
                G_Block(j, i, Dec_Clolr( Field_list[i][j] * (-1)));
                Field_list[i][j] = 0;
            }

        }
    }
}

int Normal_Tetris(int* bfore_key, int* bfore_space, MinoElement_t* MinoElement, int *bottom_y) {

    //�v�Z�t�F�[�Y

    Key_Spin(&MinoElement->Num_Spin, bfore_space);

    Dec_Block(MinoElement->Num_Spin, Next_Num_Mino_list[0], Drop_Mino_list);

    if (MinoElement->y == 0 && Is_Below(MinoElement->x, MinoElement->y)){
        function_status = 20;
        return 0;
    }

    if (Is_Below(MinoElement->x, MinoElement->y)) {
        MinoElement->x += 1;
        if (Is_Below(MinoElement->x, MinoElement->y)) {
            MinoElement->x -= 2;
            if (Is_Below(MinoElement->x, MinoElement->y)) {
                MinoElement->x -= 1;
            }
        }
        Reset_Mino_List(Drop_Mino_list);
        Dec_Block(MinoElement->Num_Spin, Next_Num_Mino_list[0], Drop_Mino_list);
    }


    Drop(&MinoElement->x, &MinoElement->y);

    Key_LR(&MinoElement->x, &MinoElement->y, bfore_key);

    Key_UP(&MinoElement->x, &MinoElement->y, bfore_key);

    Key_Fold(&MinoElement->Num_Spin);

    The_Bottom_y(MinoElement->x, MinoElement->y, bottom_y);

    switch (Is_Below(MinoElement->x, MinoElement->y)) {
    case false:
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (Drop_Mino_list[i][j] >= 1) {
                    Field_list[*bottom_y + i][MinoElement->x + j] = Drop_Mino_list[i][j] * (-1);
                    Field_list[MinoElement->y + i][MinoElement->x + j] = Drop_Mino_list[i][j];
                }
                Drop_Mino_list[i][j] = 0;
            }
        }
        break;

    case true:
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (Drop_Mino_list[i][j] >= 1) Field_list[MinoElement->y + i][MinoElement->x + j] = Drop_Mino_list[i][j] + 10;
                Drop_Mino_list[i][j] = 0;
            }
        }
        MinoElement->x = 4;
        MinoElement->y = 0;
        MinoElement->Num_Spin = 0;
        Fold_flag = 0;

        for (int i = 0; i < 4; i++) {
            Next_Num_Mino_list[i] = Next_Num_Mino_list[i + 1];
        }

        Next_Num_Mino_list[4] = GetRand(6) + 1;

        break;

    default:
        break;
    }

    Clear_Line();

    //Fold����
    Reset_Mino_List(Fold_Mino_list);
    Dec_Block( 0, Fold_Mino, Fold_Mino_list);

    //Next����
    for (int i = 0; i < 5; i++) {
        Reset_Mino_List(Next_Mino_list[i]);
        Dec_Block(0, Next_Num_Mino_list[i], Next_Mino_list[i]);
    }

    //�`��t�F�[�Y

    Draw_Field();

    //Fold
    DrawFormatString(F_Reference_Point_X, F_Reference_Point_Y -20, Clolr_White, "Fold");
    for (int j = 0; j < 4; j++) {
        for (int k = 0; k < 4; k++) {
            F_Block(k, j, Dec_Clolr(Fold_Mino_list[j][k]));
        }
    }

    //Next
    DrawFormatString(N_Reference_Point_X + 20, N_Reference_Point_Y - 20, Clolr_White, "Next");
    for (int i = 1; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                N_Block(j, k + (i - 1) * 5, Dec_Clolr(Next_Mino_list[i][k][j]));
            }
        }
    }
    return 0;
}

void White_Line() {
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);
    for (int j = 0; j < Field_Width; j++) {
        for (int i = 0; i < Field_Height * ( Block_Size + 1 ); i++) {
            DrawPixel(Reference_Point_X + j * ( Block_Size + 1 ) - 1, Reference_Point_Y + i, Clolr_White);
        }
    }

    for (int j = 0; j < Field_Width * ( Block_Size + 1 ); j++) {
        for (int i = 0; i < Field_Height; i++) {
            DrawPixel(Reference_Point_X + j, Reference_Point_Y + i * ( Block_Size + 1 ) - 1, Clolr_White);
        }
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);//�E�B���h�E���[�h�ύX�Ə������Ɨ���ʐݒ�

    MenuElement_t MenuElement[5] = {
        {  80, 100, "Normal Tetris" },
        { 100, 150, "�R�_�b�N" },
        { 100, 200, "�R�_�b�N" },
        { 100, 250, "�R�_�b�N" },
        { 100, 300, "�Q�[���I��" },
    };
    int SelectNum = 0;

    MinoElement_t MinoElement = { 4,0,0 };

    int bfore_key = 0; //�L�[�{�[�h����flag
    int bfore_space = 0; //�X�y�[�X�L�[flag
    int bottom_y = 20;

    for (int i = 0; i < 5; i++) {
        Next_Num_Mino_list[i] = GetRand(6) + 1;
    }


    Flame();

    while (ScreenFlip() == 0 && ProcessMessage() == 0 && gpUpdateKey() == 0) {

        ClearDrawScreen();

        DrawFormatString(20, 20, Clolr_White, "%d", count / 60);
        DrawFormatString(20, 50, Clolr_White, "%d", function_status);

        switch (function_status / 10) {

        case 0:

            Opning(&SelectNum, MenuElement);

            break;

        case 1:

            count += 1;

            switch (function_status) {

            case 10:
                if (count >= 60 * 3 - 1) {
                    function_status = 11;
                }

                SetFontSize(200);
                DrawFormatString(300, 100, Clolr_White, "%d", 3 - count / 60);
                SetFontSize(20);

                White_Line();
                Draw_Field();

                SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
                DrawBox(0,0,640,480,Clolr_Black,TRUE);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

                break;

            case 11:

                Normal_Tetris(&bfore_key, &bfore_space, &MinoElement, &bottom_y);

                White_Line();

                break;

            default:
                break;

            }
            break;

        case 2:

            count += 1;

            White_Line();
            Draw_Field();

            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
            DrawBox(0, 0, 640, 480, Clolr_Black, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

            SetFontSize(150);
            DrawFormatString(120, 150, Clolr_White, "�����");
            SetFontSize(20);

            break;

        default:
            DxLib_End(); // DX���C�u�����I������
            return 0;
            break;
        }
    }
    DxLib_End(); // DX���C�u�����I������
    return 0;
}