#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
        int semid; //IPC-дескриптор для массива семафоров
        char pathname[] = "for_ipc.ipc"; //Имя файла, с помощью которого генерируется ipc-кл>
        key_t key; //Переменная для хранения ipc-ключа
        struct sembuf mybuf; //Структура данных, задающая операции над определенным семафором
        //Процесс генерации ipc-ключа
        if((key = ftok(pathname,0)) <0) { 
                printf("Не удалось сгенерировать ключ ipc\n");
                exit(-1);
        }
        //Процесс получения доступа к массиву семафоров для конкретного ipc-ключа
        //Если массива семафоров не существует, то пытаемся его создать
        if((semid = semget(key, 1, 0666 | IPC_CREAT)) <0) {
                printf("Не удалось получить дескриптор массива семафоров\n");
                exit(-1);
	}
	mybuf.sem_op = 1; //Кодирование операции A(semid1,1)
	mybuf.sem_flg = 0;
	mybuf.sem_num = 0;
	//Операции будут осуществляться над семафором 0, код операции -1
	printf("Разблокировка процесса, запущенного ранее\n");
	//Выполняем операцию над семафором
	if(semop(semid, &mybuf, 1) <0) {
		printf("Не удалось выполнить операцию над семафором\n");
		exit(-1);
	}
	printf("Значение семофора установлено в единицу\n");
	return 0;
}
