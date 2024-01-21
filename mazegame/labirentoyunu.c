#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <conio.h>
#include <time.h>
#include <limits.h>
#include <math.h>


typedef struct{ // Kullanici bilgilerine skor bilgisi eklemedim cunku veritabani kullanmiyoruz, surekli users.bin klasorunu acip oradan oyunu oynayan kullanicinin struct yapisina ulasip skor guncellemenin anlami yok bosa maliyet, biz sadece top 5 skor yapabilen kullanici isimlerini halletsek yeter
    char user_real_name[30];
    char user_surname[30];
    char user_name[20];
    char password[16];
    
}USER;

void show_Main_Menu(char *decision); //Ana menuyu gosteren fonksiyon.

void show_Login_Menu(char *x); //Login menusunu gosteren fonksiyon.

void show_board(char **board,int row,int column,int *collected_pieces); //oyun tahtasini gosteren fonksiyon.

void show_board2(char **board,int row, int column); //harita secme ekraninda map'leri gosterirken kullanilan fonksiyon.

void show_Tutorial(); //Tutorial gosteren fonksiyon.

void show_Top_5(); //en iyi 5 oyuncuyu gosteren fonksiyon.

int calculate_num_karsit_hidrojen(int *collected_pieces); //uretilen karsit hidrojen sayisini hesaplayan fonksiyon.

int calculate_score(int num_karsit_hidrojen,int kalan_sure,int rows,int columns); //Skor hesapliyor oyun sonrasi.

void copy_MapToBoard(char **board,int columns, FILE *file); //Haritayi dosyadan board'a kopyalayan fonksiyon.

void create_Board(char ***board,int rows,int columns); //Board olusturan fonksiyon.

void free_board(char **board, int rows); //Boardi serbest birakan fonksiyon.

void find_Num_Boards_RowAndColumn(int *rows, int *columns, FILE *file); //Boardin row ve column sayisini bulan fonksiyon.

void get_Password(char *password); //sifre alirken kullanilan fonksiyon.

void user_Register(); //User'i kayit eden fonksiyon.

int** create_matris(int rows, int columns); //shortest_path icinde uzaklik ve visited matrisleri acarken kullanilacak fonksiyon.

void freeMatrix(int** matrix, int rows); // matris free eden fonksiyon.

USER* user_Login(); //Giris yapma kontrolu yapan fonksiyon.

void destroy_Piece(int *collected_pieces); //Bir parcacikla o parcacigin zit isaretli karsit parcacigi bir araya gelince patliyolar.

char **choose_Map(int *rows,int *columns); // map sectirten fonksiyon

void increase_piece(int *collected_pieces, int index); //toplanan parca sayisini arttiran fonksiyon.

void show_pieces(int *collected_pieces); //toplanan parcalari gosteren fonksiyon.

void find_entrance(int *x, int *y,char **board,int row,int column); //Boardi haritadan cekince giris nerede onu basta bi bulmamiz lazim.

int find_char_index(char *pieces, char piece); // gidilen sutunda parca var mi diye bakan fonksiyon.

void show_piece(int piece); // bir parcayi gosteren fonksiyon

int load_Map(char *file_name,int *rows,int *columns,char ***board); //dosyadan haritayi programa yukleyen fonksiyon.

void update_highest_scores(USER user,int curr_player_score); //oyun sonu yuksek skor guncellemesi kontrolu yapan fonksiyon.

void play(USER *user); //manuel oynama fonksiyonu

void show_Play_Menu(char *a); //play menusunu gosteren fonksiyon.

int shortest_path(char **matrix, int start_x, int start_y, int *target_x, int *target_y,int rows,int columns,int *collected_pieces,char *pieces);// auto play icin kullanilacak dijkstra algoritmasi

double calculate_distance(int currentx,int currenty, int destinationx,int destinationy);// noktalar arasi uzaklik hesabi yapan fonksiyon.

int find_closest_pieces_coord(int *x,int*y,char **board,int rows,int columns,int currentx,int currenty); // En yakindaki parcayi bulan fonksiyon, auto playde kullanilacak hep.

void auto_Play(USER *user); //otomatik oynatan fonksiyon

int main(){
    char decision,x;
   
    do{
        decision = '1';
        show_Main_Menu(&decision);
        if(decision == '1'){
            USER* user = user_Login();
            if(user != NULL){
                do{
                    x = '1';
                    show_Login_Menu(&x);
                    if(x == '1'){
                        char a = '1';
                        while(a != 27){
                            show_Play_Menu(&a);
                            if(a == '1'){
                                play(user);
                            }
                            if(a == '2'){
                                auto_Play(user);
                            }
                        }
                        
                    }
                    if(x == '2'){
                        show_Tutorial();
                    }
                    if(x == '3'){
                        show_Top_5();
                    }
                }while(x != 27);
                
            }
            free(user);
        }

        if(decision == '2'){
            user_Register();
        }

    }while(decision != 27);
        
    return 0;
}

void show_Play_Menu(char *a){
    do{
        system("cls");
        if(*a != '1' && *a != '2' && *a != 27){
            printf("Invalid choice\n");
        }
        printf("1)Play Manuel\n");
        printf("2)Auto Play\n");
        printf("ESC TO EXIT\n");
        
        *a = getch();
        
    }while(*a != '1' && *a != '2' && *a != 27);
}

void show_Tutorial(){
    system("cls");
    printf("# The main objective of the game is to collect particles that can quickly generate Antihydrogen and reach the exit.\n");
    printf("# In the maze, there are 'P+', 'P-', 'e-', and 'e+' particles. Antihydrogen can be produced from 'P-' and 'e+' particles.\n");
    printf("# Black holes are represented by 'K' on the map; entering a black hole results in elimination, and the game ends without calculating your score.\n");
    printf("# If your time runs out, you are considered to have lost, and the game ends without calculating your score. \n");
    printf("#  You need to reach the exit within the specified time and be able to create Antihydrogen to calculate your score,. ");
    printf("# Your score depends on how quickly you complete the game, the size of the maze you played in, and your capacity to generate Antihydrogen.\n# Use the arrow keys to play the game.\n");
    printf("        Press any key to continue..\n");
    getch();
}


void show_Main_Menu(char *decision){
    do{
        system("cls");
        if(*decision != '1' && *decision != '2' && *decision != 27){
            printf("Invalid choice\n");
        }
        printf("1)Login\n");
        printf("2)Register\n");
        printf("ESC TO EXIT\n");
        
        *decision = getch();
        
    }while(*decision != '1' && *decision != '2' && *decision != 27);
    
}


void show_Login_Menu(char *x){
    
    do{
        system("cls");
        if(*x != '1' && *x != '2' && *x != '3' && *x != 27){
            printf("Invalid choice\n");
        }
        printf("1) Start Game\n");
        printf("2)Tutorial\n");
        printf("3)HIGHEST 5 SCORES\n");
        *x = getch();
        
    }while(*x != '1' && *x != '2' && *x != '3' && *x !=27);
    
}


void create_Board(char ***board, int rows, int columns){
    int i;
    *board = (char**)malloc(rows*sizeof(char*));
    for(i = 0;i<rows;i++){
        (*board)[i] = (char*)malloc(columns*sizeof(char));
    }
}

void free_board(char **board, int rows){
    int i;
    for(i = 0;i<rows;i++){
        free(board[i]);
    }
    free(board);
}

void find_Num_Boards_RowAndColumn(int *rows, int *columns, FILE *file) {
    if (file == NULL) {
        printf("File is not open.\n");
        return;
    }
    fseek(file, 0, SEEK_SET);
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch != ' ' && ch != '\n' && *rows < 1) {
            (*columns)++;
        } else if (ch == '\n') {
            (*rows)++;
            
        }
    }
    (*rows)++;
  
}

void copy_MapToBoard(char **board,int columns, FILE *file){
    int i= 0,j = 0;
    char ch;
    fseek(file, 0, SEEK_SET);
    while((ch = fgetc(file)) != EOF){
        if (ch != ' ' && ch != '\n') {
            board[i][j] = ch;
            j++;
            if (j == columns) {
                j = 0;
                i++;
            }
        }
    }
    fclose(file);
}

int load_Map(char *file_name,int *rows,int *columns,char ***board){
    FILE *file;
    file = fopen(file_name,"r+");
    if(file == NULL){
        printf("Boyle bir map yok.");
        return 0;
    }
    find_Num_Boards_RowAndColumn(rows,columns,file);
    create_Board(board,*rows,*columns);
    copy_MapToBoard(*board,*columns,file); //dosyayi burada kapatiyoruz.
    return 1;

}

void find_entrance(int *x, int *y,char **board,int row,int column){
    while(*x < row){
        *y = 0;
        while(*y < column){
            if(board[*x][*y] == 'G'){
                return;
            }
            (*y)++;
        }
        (*x)++;
    }
}

int find_char_index(char *pieces, char piece){
    int i;
    for(i = 0; i <sizeof(pieces);i++){
        if(pieces[i] == piece){
            return i; // bulundu, i indeksinde.
        }
    }
    return -1; //bulunamazsa -1 don.
}

void show_piece(int piece){
    if(piece == 0){
        printf("P+ ");
    }
    if(piece == 1){
        printf("e- ");
    }
    if(piece == 2){
        printf("P- ");
    }
    if(piece == 3){
        printf("e+ ");
    }
}

void show_pieces(int *collected_pieces){
    int i,j;
    for(i  = 0;i<4;i++){//4 parcamiz var
        for(j = 0;j<collected_pieces[i];j++){ //collected_pieces bir parcadan kac tane toplandigini gosteriyor.
            show_piece(i);
        }
    }
}

void increase_piece(int *collected_pieces, int index){
    collected_pieces[index]++; //belirli bir parcadan toplaninca o parcadan 1 adet artiyor.
}

void show_board(char **board,int row, int column,int *collected_pieces){
    system("cls");
    int i,j;
    int char_width = 2;
    printf("\n\n\n\n");
    for(i = 0;i<row;i++){
        if(i == 0){
            printf("             ");
            for(j = 0;j<column;j++){
                printf("%-*s", char_width, "__");
            }
            printf("\n");
        }
        for(j = 0;j<column;j++){
            if(j == 0){
                printf("            ");
                printf("%-*c", char_width, '|');
            }
            if (board[i][j] != '0'&& board[i][j] != '1') {
                if(board[i][j] == 'P'){
                    printf("%-*s", char_width, "P+");
                }
                else if(board[i][j] == 'p'){
                    printf("%-*s", char_width, "P-");
                }
                else if(board[i][j] == 'E'){
                    printf("%-*s", char_width, "e+");
                }
                else if(board[i][j] == 'e'){
                    printf("%-*s", char_width, "e-");
                }
                else{
                    printf("%-*c", char_width, board[i][j]);
                }
                
            }
            else if(board[i][j] == '1'){
                printf("%-*c", char_width, '|');
            }
            else {
                printf("%*c", char_width, ' ');
            }
            if(j == column-1){
                printf("%-*c", char_width, '|');
            }

        }
        if(i == 0){
            printf("         Toplanan atom alti parcaciklar:");
        }
        if(i == 1){
            printf("        ");
            show_pieces(collected_pieces);
        }
        if(i == row-1){
            printf("\n");
            printf("             ");
            for(j = 0;j<column;j++){
                printf("%-*s", char_width, "--");
            }  
        }
        printf("\n");
    }
}

void destroy_Piece(int *collected_pieces){ //parca yok etme fonksiyonu.
    //P- ile P+ veya e- ile e+ yanyana geldigi an birbirlerini yok ederler. Oyun kurallari geregi
    if(collected_pieces[0] >= collected_pieces[2]){
        collected_pieces[0] = collected_pieces[0] - collected_pieces[2];
        collected_pieces[2] = 0;
    }
    if(collected_pieces[0] < collected_pieces[2]){
        collected_pieces[2] = collected_pieces[2] - collected_pieces[0];
        collected_pieces[0] = 0;
    }
    if(collected_pieces[1] >= collected_pieces[3]){
        collected_pieces[1] = collected_pieces[1] -collected_pieces[3];
        collected_pieces[3] = 0;
    }
    if(collected_pieces[1] < collected_pieces[3]){
        collected_pieces[3] = collected_pieces[3] -collected_pieces[1];
        collected_pieces[1] = 0;
    }
    
}

void show_board2(char **board,int row, int column){
    system("cls");
    int i,j;
    int char_width = 2;
    printf("\n\n\n\n");
    for(i = 0;i<row;i++){
        if(i == 0){
            printf("             ");
            for(j = 0;j<column;j++){
                printf("%-*s", char_width, "__");
            }
            printf("\n");
        }
        for(j = 0;j<column;j++){
            if(j == 0){
                printf("            ");
                printf("%-*c", char_width, '|');
            }
            if (board[i][j] != '0'&& board[i][j] != '1') {
                if(board[i][j] == 'P'){
                    printf("%-*s", char_width, "P+");
                }
                else if(board[i][j] == 'p'){
                    printf("%-*s", char_width, "P-");
                }
                else if(board[i][j] == 'E'){
                    printf("%-*s", char_width, "e+");
                }
                else if(board[i][j] == 'e'){
                    printf("%-*s", char_width, "e-");
                }
                else{
                    printf("%-*c", char_width, board[i][j]);
                }
                
            }
            else if(board[i][j] == '1'){
                printf("%-*c", char_width, '|');
            }
            else {
                printf("%*c", char_width, ' ');
            }
            if(j == column-1){
                printf("%-*c", char_width, '|');
            }
            
           
            
        
        }
        if(i == row-1){
            printf("\n");
            printf("             ");
            for(j = 0;j<column;j++){
                printf("%-*s", char_width, "--");
            }  
        }
        printf("\n");
    }
}

char **choose_Map(int *rows,int *columns){
    char a = '0',b;
    char **board = NULL;
    while(a != 27){
        system("cls");
        printf("1) Hazir haritalar\n");
        printf("2) Kendi haritani yukle.\n");
        printf("Geri gelmek icin esc\n");
        a = getch();
        if(a == '1'){
            int i = 1;
            char file_name[30];
            b = '0';
            while(i>0 && i<5 && b !=27){
                snprintf(file_name,sizeof(file_name),"map%d.txt",i);
                if (board != NULL) {
                    free_board(board, *rows);
                    board = NULL;
                }
                if(*rows !=  0 || *columns != 0){
                    *rows =0;
                    *columns = 0;
                }
                if (load_Map(file_name, rows, columns, &board)) {
                    show_board2(board, *rows, *columns);
                    printf("Map'ler arasi gecis yapmak icin -> veya <- , Map secmek icin enter'a basin. Geri gelmek icin ESC \n");
                    b = getch();
                    if(b == 224 || b == 0 || b == -32){ //yon tuslarinda ilk basta ASCII degerlerinde 224 veya 0 doner numlock acik kapali olmasina gore sonra yon tusu asciisini doner. -32 donuyor bende
                        b = getch(); //yon tusu girilince 2 kez okuma yapiyor bende ilk ascii olarak -32 okuyordu sonra yon tusu ascii degerini okuyor.
                        if (b == 77 && i <= 3) {
                            i++;
                        }
                        if (b == 75 && i >= 2) {
                            i--;
                        }
                        
                    }
                    if (b == 13) {//enter tusu girildiyse.
                        printf("Secilen board yukleniyor...\n");
                        sleep(1);
                        return board;
                    }
                } 
                else {//harita yuklenemezse
                    printf("Harita yuklenemedi.\n");
                    return NULL;
                }
                
            }
            
            
        }
        if(a == '2'){
            if (board != NULL) {
                    free_board(board, *rows);
                    board = NULL;
                }
            system("cls");
            char file_name2[20];
            printf("Yuklemek istediginiz haritanin tam adini giriniz .txt uzantisi dahil.\n"); //txt uzantisi ile girilecek harita yazilmali.
            scanf("%s",file_name2);
            if (load_Map(file_name2, rows, columns, &board)){
                printf("Haritaniz yukleniyor ...\n");
                sleep(1);
                return board;
            }
            else{
                printf("Boyle bir harita yok kontrol ediniz.\n");
                sleep(1);
                
            }
        }
    }
    return NULL;
    
}

void show_Top_5(){
    FILE *file = fopen("highest_scores.txt","a+");//dosya yoksa ac
    if(file == NULL){
        perror("Dosya acilamadi!\n");
        exit(1);
    }
    int row = 0,i = 0;
    int highest_scores[5] = {0};
    char user_names[5][30];
    while(row <5 && fscanf(file,"%s %d",user_names[row],&highest_scores[row]) == 2){
        row++;
    }
    system("cls");
    printf("En Yuksek Puanli 5 Oyuncu:\n");
    if(row == 0){ //Eger oyuncu yoksa bu sekilde goster. Bunun asil amaci kullanici oyuna girer girmez hic oynamadan show rank derse.
        for(i = 0;i<5;i++){
            printf("%d NULL->0 point\n",i+1);
        }
        
    }
    for(i = 0;i<row;i++){
        printf("%d %s->%d point\n",i+1,user_names[i],highest_scores[i]);
    }
    fclose(file);
    getch();

}

int calculate_num_karsit_hidrojen(int *collected_pieces){

    return (collected_pieces[2]<collected_pieces[3]) ? collected_pieces[2] : collected_pieces[3]; 

}

int calculate_score(int num_karsit_hidrojen,int kalan_sure,int rows,int columns){
    int score = num_karsit_hidrojen * kalan_sure * kalan_sure *rows *columns;
    return score;
}


void update_highest_scores(USER user,int curr_player_score){
    FILE *file = fopen("highest_scores.txt","a+"); //dosya yoksa da olustursun.
    if(file == NULL){
        perror("Dosya acilamadi!\n");
        exit(1);
    }
    int row = 0,j = 0,i = 0;
    typedef struct TOP5{
        int score;
        char user_name[30];
    }TOP5;
    
    TOP5 top5[5];
    char buffer[100];
    if(fscanf(file,"%s %d",top5[row].user_name,&top5[row].score)!= 2){ //ilk oynanista bossa yaz don.
        fclose(file);
        file = fopen("highest_scores.txt","w");
        fprintf(file,"%s %d\n",user.user_name,curr_player_score);
        for(j = 0;j<4;j++){
            fprintf(file,"NULL 0\n"); //Eger yeni acildiysa dosyayi bunla doldur.
        }
        fclose(file);
        return;
    }
    fseek(file, 0, SEEK_SET);
    while(fgets(buffer, sizeof(buffer), file) != NULL){
        sscanf(buffer, "%s %d", top5[row].user_name, &top5[row].score);
        row++;
    }
    
    fclose(file);
    int flag = 0;
    while(flag == 0 && i<5){
        if(curr_player_score >= top5[i].score){
            for(j = row-1;j>i;j--){
                top5[j] = top5[j-1];
            }
            strcpy(top5[i].user_name,user.user_name);
            top5[i].score = curr_player_score;
            flag =1;
        }
        
        i++;
    }
  
    file = fopen("highest_scores.txt","w");
    for(i = 0;i<row;i++){
        
        fprintf(file,"%s %d\n",top5[i].user_name,top5[i].score);
        
    }
    fclose(file);
}

void play(USER *user){
    system("cls");
    char pieces[4] = {'P','e','p','E'};
    int collected_pieces[4] = {0};
    char **board,action;
    int i = 0,j = 0;
    int rows = 0,columns = 0;
    board = choose_Map(&rows,&columns);
    int sure_limit = (rows+columns)/2; //sure harita buyuklugune gore degisiyor.
    double gecen_sure;
    int kalan_sure;
    if(board != NULL){ //board yuklenebildiyse oyun baslar.
        find_entrance(&i,&j,board,rows,columns);
        int temp_i = i, temp_j = j;//burasi ilk Hamle sonrasi kontrol icin G'yi matriste yerine geri koycaz.
        board[i][j] = 'X';
        show_board(board,rows,columns,collected_pieces);
        int flag = 0;
        clock_t start_time = clock(); //oyuna baslanan saati kaydediyoruz.
        while(flag != -1 && flag != -2 && flag != -3 && action != 27 && flag != -4){ //escye basildigi an oyundan cikilabilir.
             
            if(board[temp_i][temp_j] != 'X'){
                board[temp_i][temp_j] = 'G';
            }
            show_board(board,rows,columns,collected_pieces);
            clock_t gecen_zaman = clock() - start_time;
            gecen_sure = ((double)gecen_zaman) / CLOCKS_PER_SEC;
            kalan_sure = sure_limit - (int)gecen_sure; //kalan sureyi hesaplayip ekrana bastiriyoruz.
            printf("Kalan Sure: %d saniye\n", kalan_sure);
            flag = 0;
            if (gecen_sure >= sure_limit) {
                flag = -4;  //sure bittiyse flag direkt -4 doner ve oyun sonlanir.
               
            }
            
            while(flag == 0 && action != 27){ 
                action = getch();
                if(action == 27){
                    flag = -3; //escye basildiysa direkt oyundan cikilir.
                }
                if(action == 224 || action == 0 || action == -32){ //yon tusu kontrolu, yon tusuna ilk basilista bende ilk -32 okuyor daha sonra basilan tusun asciisini okuyor.
                    action = getch();
                    if(action == 77 && j+1<columns){
                        if(board[i][j+1] == 'K'){
                            board[i][j] = '0';
                            flag = -2; //karadelige girildiyse flag -2 doner ve oyun biter
                        }
                        else if(board[i][j+1] == 'C'){
                            board[i][j] = '0';
                            board[i][j+1] = 'X';
                            flag = -1; //cikisa ulasildiysa flag -1 doner ve oyun biter
                        }
                        else if(board[i][j+1] != '1'){
                            j++;
                            int index = find_char_index(pieces,board[i][j]);
                            if(index != -1){
                                increase_piece(collected_pieces,index);
                                destroy_Piece(collected_pieces);
                            }
                            board[i][j] = 'X';
                            board[i][j-1] = '0';
                            flag = 1; //oyuna devam
                        } 
                    }
                    if(action == 72 && i-1 >= 0){
                        if(board[i-1][j] == 'K'){
                            board[i][j] = '0'; //karadelik X'i yutar
                            flag = -2;
                        }
                        else if(board[i-1][j] == 'C'){
                            board[i-1][j] = 'X';
                            board[i][j] = '0';
                            flag = -1;
                        }
                        else if(board[i-1][j] != '1'){
                            i--;
                            int index = find_char_index(pieces,board[i][j]);
                            if(index != -1){
                                increase_piece(collected_pieces,index);
                                destroy_Piece(collected_pieces);
                            }
                            board[i][j] = 'X';
                            board[i+1][j] = '0';
                            flag = 1;
                        } 
                    }
                    if(action == 75 && j-1 >= 0){
                        if(board[i][j-1] == 'K'){
                            board[i][j] = '0';
                            flag = -2;
                        }
                        else if(board[i][j-1] == 'C'){
                            board[i][j-1] = 'X';
                            board[i][j] = '0';
                            flag = -1;
                        }
                        else if(board[i][j-1] != '1'){
                            j--;
                            int index = find_char_index(pieces,board[i][j]);
                            if(index != -1){
                                increase_piece(collected_pieces,index);
                                destroy_Piece(collected_pieces);
                            }
                            board[i][j] = 'X';
                            board[i][j+1] = '0';
                            flag = 1;
                        } 
                    }
                    if(action == 80 && i+1 < rows){
                        if(board[i+1][j] == 'K'){
                            board[i][j] = '0';
                            flag = -2;
                        }
                        else if(board[i+1][j] == 'C'){
                            board[i+1][j] = 'X';
                            board[i][j] = '0';
                            flag = -1;
                        }
                        else if(board[i+1][j] != '1'){
                            i++;
                            int index = find_char_index(pieces,board[i][j]);
                            if(index != -1){
                                increase_piece(collected_pieces,index);
                                destroy_Piece(collected_pieces);
                            }
                            board[i][j] = 'X';
                            board[i-1][j] = '0';
                            flag = 1;
                        } 
                    }           
                }
            }
        }   
        if(flag == -2){
            show_board(board,rows,columns,collected_pieces);
            printf("Karadelige girdiniz ve kaybettiniz.\n");
            printf("Press any key to continue...\n");
            getch();
        }
        if(flag == -1){
            show_board(board,rows,columns,collected_pieces);
            printf("Cikisa ulasildi. ");
            printf("%d adet karsit madde uretildi\n",calculate_num_karsit_hidrojen(collected_pieces));
            printf("Arta kalan sure: %d\n",kalan_sure);
            int player_new_score = calculate_score(calculate_num_karsit_hidrojen(collected_pieces),kalan_sure,rows,columns);
            printf("Kazanilan skor : %d\n",player_new_score);
            update_highest_scores(*user,player_new_score);
            printf("Press any key to continue...\n");
            getch();
        }
        if(flag == -4){
            printf("Sureniz bitti basaramadiniz..\n");
            printf("Press any key to continue...\n");
            getch();
        }
        free_board(board,rows);
    }


}

void get_Password(char *password){
    char ch;
    ch = getchar();
    int i = 0;
    do{
        ch = getch(); 
        if (ch == 13) { 
            password[i] = '\0'; 
           
        }
        else if (ch == 8 || ch == 127) {
            if (i > 0) {
                printf("\b \b");  
                i--;
            }
        }
        else if (i < 15) {
            password[i] = ch;
            printf("*");
            i++;
        }
    }while(ch !=13);
}

USER* user_Login(){
    FILE *file = fopen("users.bin","rb");
    if(file == NULL){
        printf("Once kayit olmalisiniz. En az bir hesap olmali\n");
        perror("FILE opening error!\n");
        exit(1);
    }
    char user_name[30];
    char password[16];
    system("cls");
    USER* temp_user = (USER*)malloc(sizeof(USER));
    int count = 3; //deneme hakki 3 kez yanlis girilirse ana menuye geri doner.
    USER temp_data;
    while(count > 0 ){
        fseek(file,0,SEEK_SET);
        printf("Username: ");
        scanf("%s",user_name);
        printf("Password:");
        get_Password(password);
        while(fread(&temp_data, sizeof(USER), 1, file) == 1){
            if (strcmp(temp_data.user_name, user_name) == 0 && strcmp(temp_data.password,password) == 0) {
                printf("\nSuccesfully Entered\n");
                memcpy(temp_user, &temp_data, sizeof(USER));
                sleep(1);
                fclose(file);
                return temp_user;
            }
        }
        count--;
        printf("\nYour username or password is wrong.\n");
        printf("Kalan deneme hakki: %d\n",count);
    }
    fclose(file);
    system("cls");
    printf("Deneme hakkiniz bitti menuye donduruluyorsunuz....\n");
    sleep(2);
    free(temp_user);
    return NULL;
    
}

void user_Register(){
    system("cls");
    FILE *file = fopen("users.bin","ab+");
    if(file == NULL){
        perror("FILE opening error!\n");
        exit(1);
    }
    USER user;
    char user_real_name[30];
    char user_surname[30];
    char user_name[20];
    char password[16];
    printf("Please enter your name: ");
    fgets(user_real_name, sizeof(user_real_name), stdin);
    user_real_name[strcspn(user_real_name, "\n")] = '\0';
    printf("Please enter your surname: ");
    scanf("%s",user_surname);
    USER temp_user;
    int flag = 0;
    
    while(flag == 0){
        fseek(file,0,SEEK_SET);
        flag = 1;
        printf("Please enter the user name: ");
        scanf("%s",user_name);
        while(fread(&temp_user, sizeof(USER), 1, file) == 1){
            if (strcmp(temp_user.user_name, user_name) == 0) {
                printf("The username is already in use. Please enter a different username.\n");
                flag = 0;
                
            }
        }
    }
    printf("Password:");
    get_Password(password);
    strcpy(user.user_name,user_name);
    strcpy(user.user_real_name,user_real_name);
    strcpy(user.user_surname,user_surname);
    strcpy(user.password,password);
    fseek(file,0,SEEK_END);
    fwrite(&user,sizeof(USER),1,file);

    fclose(file);
    
    printf("\nRegistered succesfully.\n");
    printf("Press any key to continue....\n");
    getchar();


    

}

int** create_matris(int rows, int columns) {
    int** matris;
    int i;

    matris = (int**)malloc(rows * sizeof(int*));

    for (i = 0; i < rows; i++) {
        matris[i] = (int*)malloc(columns * sizeof(int));
    }

    return matris;
}

void freeMatrix(int** matrix, int rows) {
   int i;
    for (i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

//bu shortest_path e- ve p+ ve 1 ve K hedeflerini duvar gibi algiliyor. Ayrica haritada asla ulasamayacaðý engeller tarafýndan kapatilmis bir koordinata gidemez.
int shortest_path(char **matrix, int start_x, int start_y, int *target_x, int *target_y,int rows,int columns,int *collected_pieces,char *pieces) {//dijkstra algoritmasi uyarladim.
    //tersten gittigimiz icin start_x ve target_x i degistiricez bu fonksiyona verirken.
    int **distances = create_matris(rows,columns); //uzakliklari tuttugumuz matrix

    int **visited = create_matris(rows,columns); //ziyaret edilen hucreleri tuttugumuz matrix.
	int i,j,count;
    int startNode = start_x * columns + start_y;
    int targetNode = *target_x * columns + *target_y;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < columns; j++) {
            distances[i][j] = INT_MAX; //uzakliklari en basta tum hucrelere sonsuz uzaklik olarak atiyoruz.
            visited[i][j] = 0; //ziyaret edilenler baslangicta bos.
        }
    }

    distances[start_x][start_y] = 0; //baslangic hucresinin uzakligi 0 atiyoruz.

    for (count = 0; count < rows * columns - 1; count++) {
        int min_distance = INT_MAX;
        int min_i, min_j;

        for (i = 0; i < rows; i++) {
            for (j = 0; j < columns; j++) {
                if (!visited[i][j] && distances[i][j] < min_distance) {
                    min_distance = distances[i][j];
                    min_i = i;
                    min_j = j;
                }
            }
        }
        visited[min_i][min_j] = 1;

        if (min_i > 0 && matrix[min_i - 1][min_j] != '1' && distances[min_i - 1][min_j] > distances[min_i][min_j] + 1 && matrix[min_i - 1][min_j] != 'K' && matrix[min_i - 1][min_j] != 'e' && matrix[min_i - 1][min_j] != 'P') {
            distances[min_i - 1][min_j] = distances[min_i][min_j] + 1;
        }
        if (min_i < rows - 1 && matrix[min_i + 1][min_j] != '1' && distances[min_i + 1][min_j] > distances[min_i][min_j] + 1 && matrix[min_i + 1][min_j] != 'K' && matrix[min_i + 1][min_j] != 'e' && matrix[min_i + 1][min_j] != 'P') {
            distances[min_i + 1][min_j] = distances[min_i][min_j] + 1;
        }
        if (min_j > 0 && matrix[min_i][min_j - 1] != '1' && distances[min_i][min_j - 1] > distances[min_i][min_j] + 1 && matrix[min_i][min_j-1] != 'K'&& matrix[min_i][min_j-1] != 'e' && matrix[min_i][min_j-1] != 'P') {
            distances[min_i][min_j - 1] = distances[min_i][min_j] + 1;
        }
        if (min_j < columns - 1 && matrix[min_i][min_j + 1] != '1' && distances[min_i][min_j + 1] > distances[min_i][min_j] + 1 && matrix[min_i][min_j+1] != 'K'&& matrix[min_i][min_j+1] != 'e' && matrix[min_i][min_j+1] != 'P') {
            distances[min_i][min_j + 1] = distances[min_i][min_j] + 1;
        }
    }

    
    int current_X = *target_x;
    int current_Y = *target_y;

    while (current_X != start_x || current_Y != start_y) {
        
        show_board(matrix,rows,columns,collected_pieces);
        printf("\n");
       
        if (current_X > 0 && distances[current_X - 1][current_Y] == distances[current_X][current_Y] - 1) {
            current_X--;
            if(matrix[current_X][current_Y] == 'C'){ //cikissa -1 don ciktiginin farkina varsin algoritma
                matrix[current_X][current_Y] = 'X';
                matrix[current_X+1][current_Y] = '0';
                return -1;
            }
            int index = find_char_index(pieces,matrix[current_X][current_Y]);
            if(index != -1){
                increase_piece(collected_pieces,index);
                destroy_Piece(collected_pieces);
            }
            
            matrix[current_X][current_Y] = 'X';
            matrix[current_X+1][current_Y] = '0';
           
        } else if (current_X < rows - 1 && distances[current_X + 1][current_Y] == distances[current_X][current_Y] - 1) {
            current_X++;
            if(matrix[current_X][current_Y] == 'C'){
                matrix[current_X][current_Y] = 'X';
                matrix[current_X-1][current_Y] = '0';
                return -1;
            }
            int index = find_char_index(pieces,matrix[current_X][current_Y]);
            if(index != -1){
                increase_piece(collected_pieces,index);
                destroy_Piece(collected_pieces);
            }
            matrix[current_X][current_Y] = 'X';
            matrix[current_X-1][current_Y] = '0';
           
        } else if (current_Y > 0 && distances[current_X][current_Y - 1] == distances[current_X][current_Y] - 1) {
            current_Y--;
            if(matrix[current_X][current_Y] == 'C'){
                matrix[current_X][current_Y] = 'X';
                matrix[current_X][current_Y+1] = '0';
                return -1;
            }
            int index = find_char_index(pieces,matrix[current_X][current_Y]);
            if(index != -1){
                increase_piece(collected_pieces,index);
                destroy_Piece(collected_pieces);
            }
            matrix[current_X][current_Y] = 'X';
            matrix[current_X][current_Y+1] = '0';
           
        } else if (current_Y < columns - 1 && distances[current_X][current_Y + 1] == distances[current_X][current_Y] - 1) {
            current_Y++;
            if(matrix[current_X][current_Y] == 'C'){
                matrix[current_X][current_Y] = 'X';
                matrix[current_X][current_Y-1] = '0';
                return -1;
            }
            int index = find_char_index(pieces,matrix[current_X][current_Y]);
            if(index != -1){
                increase_piece(collected_pieces,index);
                destroy_Piece(collected_pieces);
            }
            matrix[current_X][current_Y] = 'X';
            matrix[current_X][current_Y-1] = '0';
            
        }
        usleep(125000);
    }
    
    show_board(matrix,rows,columns,collected_pieces);
    *target_x = current_X;
    *target_y = current_Y;
    freeMatrix(distances,rows);
    freeMatrix(visited,rows);
    return 1;
}

double calculate_distance(int currentx,int currenty, int destinationx,int destinationy){
    return sqrt((currentx-destinationx)*(currentx-destinationx)+(currenty-destinationy)*(currenty-destinationy));
}

int find_closest_pieces_coord(int *x,int*y,char **board,int rows,int columns,int currentx,int currenty){
    int i = 0, j = 0;
    double closest_point = 100000;
    int flag = 0;
    double new_closest_point;
    for(i = 0;i<rows;i++){
        for(j = 0;j<columns;j++){
            if(board[i][j] == 'p' || board[i][j] == 'E'){ // sadece p- ve e+ toplicagimiz icin  onlari buluyoruz.
                flag = 1;
                new_closest_point = calculate_distance(currentx,currenty,i,j);
                if(new_closest_point< closest_point){
                    *x = i;
                    *y = j;
                    closest_point = new_closest_point;
                }
            }
        }
    }
    if(flag == 1){
        return 1;//parcacik buldu.
    }
    else if(flag == 0){ //Haritada parcacik kalmadiysa direktmen cikisa yonelecek.
        for(i = 0;i<rows;i++){
            for(j = 0;j<columns;j++){
                if(board[i][j] == 'C'){
                    double new_closest_point = calculate_distance(currentx,currenty,i,j);
                    if(new_closest_point< closest_point){
                        *x = i;
                        *y = j;
                        closest_point = new_closest_point;
                    }
                }
            }
        }
    }
    return 0;//cikisa yonelindi.
    
}


void auto_Play(USER *user){
    system("cls");
    char pieces[4] = {'P','e','p','E'};
    int collected_pieces[4] = {0};
    char **board,action;
    int rows = 0,columns = 0;
    board = choose_Map(&rows,&columns);
    int control,control2;
    int destinationx = 0,destinationy = 0,currentx = 0,currenty = 0;
    find_entrance(&currentx,&currenty,board,rows,columns);
    char a;
    if(board != NULL){
        do{
            if(kbhit()){
                a = getch();
            }
            control = find_closest_pieces_coord(&destinationx,&destinationy,board,rows,columns,currentx,currenty); //en yakindaki parcayi buluyoruz ve o parcaya dogru en kisa yoldan gidiyoruz.
            control2 = shortest_path(board,destinationx,destinationy,&currentx,&currenty,rows,columns,collected_pieces,pieces);




        }while(control == 1 && control2 == 1 && a != 27);

        if(control == 0 && a != 27){
            show_board(board,rows,columns,collected_pieces);
            printf("Cikisa ulasildi. ");
            printf("%d adet karsit madde uretildi\n",calculate_num_karsit_hidrojen(collected_pieces));
            printf("Otomatik oynama modunda puaniniz hesaplanmaz.\n");
            printf("Press any key to continue...\n");
            getch();
        }
        else if(control2 == -1 && a !=27){
            show_board(board,rows,columns,collected_pieces);
            printf("Cikisa ulasildi. ");
            printf("%d adet karsit madde uretildi\n",calculate_num_karsit_hidrojen(collected_pieces));
            printf("Otomatik oynama modunda puaniniz hesaplanmaz.\n");
            printf("Press any key to continue...\n");
            getch();
        }
    }



}      





