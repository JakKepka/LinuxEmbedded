#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <periphery/gpio.h>

gpio_t *paper, *rock, *scissors;
gpio_t *win, *lose, *draw;

void lightLed(gpio_t *gpio_led)
{

    if(gpio_write(gpio_led, 1) < 0)
    {
        fprintf(stderr, "gpio_write(): %s\n", gpio_errmsg(gpio_led));
        exit(1);
    }
    sleep(1);

    if(gpio_write(gpio_led, 0) < 0)
    {
        fprintf(stderr, "gpio_write(): %s\n", gpio_errmsg(gpio_led));
        exit(1);
    }
    sleep(1);
}

int checkButtons()
{
    int value;
    if(gpio_read(paper, &value) < 0)
    {
        fprintf(stderr, "gpio_read(): %s\n", gpio_errmsg(paper));
        exit(1);        
    }

    if(value == 0)
        return 1;

    if(gpio_read(rock, &value) < 0)
    {
        fprintf(stderr, "gpio_read(): %s\n", gpio_errmsg(rock));
        exit(1);        
    }

    if(value == 0)
        return 2;

    if(gpio_read(scissors, &value) < 0)
    {
        fprintf(stderr, "gpio_read(): %s\n", gpio_errmsg(scissors));
        exit(1);        
    }

    if(value == 0)
        return 3;

}

int main()
{
    // Papier - 1  Kamień - 2 Nożyce - 3
    // 1 > 2 > 3 > 1 

    printf("Witaj w grze Papier, Kamień i Nożyce!\n");
    printf("Papier - 1    Kamień - 2   Nożyce - 3!\n");
    printf("Papier owija kamień, kamień łamie nożyce oraz nożyce tną papier!\n");

    paper = gpio_new();
    rock = gpio_new();
    scissors = gpio_new();
    
    win = gpio_new();
    lose = gpio_new();
    draw = gpio_new();

    if(gpio_open(paper, "/dev/gpiochip0", 18, GPIO_DIR_IN) < 0)
    {
        fprintf(stderr, "gpio_open(): %s\n", gpio_errmsg(paper));
        exit(1);
    }

    if(gpio_open(rock, "/dev/gpiochip0", 17, GPIO_DIR_IN) < 0)
    {
        fprintf(stderr, "gpio_open(): %s\n", gpio_errmsg(rock));
        exit(1);
    }

    if(gpio_open(scissors, "/dev/gpiochip0", 10, GPIO_DIR_IN) < 0)
    {
        fprintf(stderr, "gpio_open(): %s\n", gpio_errmsg(scissors));
        exit(1);
    }


    if(gpio_open(win, "/dev/gpiochip0", 22, GPIO_DIR_OUT) < 0)
    {
        fprintf(stderr, "gpio_open(): %s\n", gpio_errmsg(win));
        exit(1);
    }

    if(gpio_open(lose, "/dev/gpiochip0", 27, GPIO_DIR_OUT) < 0)
    {
        fprintf(stderr, "gpio_open(): %s\n", gpio_errmsg(lose));
        exit(1);
    }

    if(gpio_open(draw, "/dev/gpiochip0", 23, GPIO_DIR_OUT) < 0)
    {
        fprintf(stderr, "gpio_open(): %s\n", gpio_errmsg(draw));
        exit(1);
    }

    // Akcją jest puszczenie danego przycisku
    gpio_edge_t edge = GPIO_EDGE_FALLING;
    gpio_set_edge(paper, edge);
    gpio_set_edge(rock, edge);
    gpio_set_edge(scissors, edge);

    gpio_t* buttons[] = {paper, rock, scissors};
    size_t count = 3;

    srand(time(NULL));

    bool ready[3];
    
    int win_count = 0;
    int lose_count = 0;
    int draw_count = 0;
    int round = 1;

    int computer_pick = 0;

    while(true)
    {
        printf("Runda %d. \n", round);

        sleep(1);

        computer_pick = rand() % 3 + 1;

        printf("Czekamy na wybór użytkownika.\n");
        int ret = gpio_poll_multiple(buttons, count, -1, ready);
        
        if(ret > 0){
            ret = checkButtons();
        }

        printf("Komputer wylosował: %d\n", computer_pick);
        printf("Wybrałeś: %d\n", ret);
        // Papier - 1  Kamień - 2 Nożyce - 3
        // 1 > 2 > 3 > 1 

        // REMIS
        if(ret == computer_pick){
            printf("Remis\n");
            draw_count++;
            lightLed(win);
        }
        // Wygrana 
        else if((ret == 1 && computer_pick == 2 ) || (ret == 2 && computer_pick == 3) || (ret == 3 && computer_pick == 1))
        {
            printf("Runda wygrana\n");
            win_count++;
            lightLed(lose);
        }
        // Przegrana 
        else{
           
            printf("Runda przegrana\n");
            lose_count++;
            lightLed(draw);
        }

        while(true)
                {
                    int ret2= gpio_poll_multiple(buttons, count, 0, ready);
                    if(ret2 <= 0)
                        break;

                    if(ready[0])
                    {
                       gpio_read_event(paper, NULL, 0);
                    }
                    if(ready[1])
                    {
                        gpio_read_event(rock, NULL, 0);
                    }
                    if(ready[2])
                    {
                        gpio_read_event(scissors, NULL, 0);
                    }
                        
                }

        printf("Koniec rundy %d.\n",round++);
        printf("Wynik (W/D/L): %d  %d  %d \n", win_count, draw_count, lose_count);
        printf("\n");
    }
}