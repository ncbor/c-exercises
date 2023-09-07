#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH (1390)
#define WINDOW_HEIGHT (900) //PADRAO FIFA KKKKKKK 105/68
#define SPEED (300)


int main(void)
{
  //Initialize TTF
  if(TTF_Init()!=0)
  {
    printf("Erro ao inicializar TTF: %s\n", TTF_GetError());
    return 1;
  }

  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS)!=0)
  {
    printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
    return 1;
  }

  //Create Window
  SDL_Window* wmain = SDL_CreateWindow(
    "PONG THE GAME",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH,WINDOW_HEIGHT,0);//SDL_WINDOW_FULLSCREEN_DESKTOP
  if(!wmain)
  {
    printf("Erro ao gerar janela: %s\n",SDL_GetError());
    return 1;
  }

  //Create Renderer
  Uint32 renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  SDL_Renderer *rmain = SDL_CreateRenderer(wmain,-1,renderer_flags);
  if(!rmain)
  {
    printf("Erro ao gerar renderizador: %s\n",SDL_GetError());
    SDL_DestroyWindow(wmain);
    return 1;
  }

  //Give color (black) to the renderer's background
  SDL_RenderClear(rmain);
  SDL_RenderPresent(rmain);

  //game starts here
  //creating 2 red paddles and ball
  SDL_Rect paddle1, paddle2, ball, comp;
  paddle1.x = 60;
  paddle1.w = 10;
  paddle1.h = 100;
  paddle1.y = (WINDOW_HEIGHT - paddle1.h)/2;
  paddle2.w = 10;
  paddle2.h = 100;
  paddle2.x = (WINDOW_WIDTH - 60 - paddle2.w);
  paddle2.y = (WINDOW_HEIGHT - paddle1.h)/2;
  ball.w=12.5;ball.h=12.5;
  ball.y=(WINDOW_HEIGHT-ball.h)/2;ball.x=(WINDOW_WIDTH-ball.w)/2;

  //drawing they into the render
  SDL_SetRenderDrawColor(rmain,10,160,10,1);
  SDL_RenderFillRect(rmain,&paddle1);
  SDL_RenderFillRect(rmain,&paddle2);
  SDL_RenderFillRect(rmain,&ball);
  SDL_RenderPresent(rmain);

  // SDL_Delay(5000);
  float y1 = paddle1.y;
  int aux = 0;

  //INPUT TRACKING EACH PADDLE
  int up1 = 0, up2 = 0;
  int down1 = 0, down2 = 0;
  int left = 0,right = 0;

  //TRACKING BALL VELOCITY
  float ball_vx=0, ball_vy=0, ball_speed = WINDOW_WIDTH/900, alpha;
  printf("%lf\n",ball_speed);//=1

  //loop delay
  int delay = 10;

  //points
  int p1 = 0, p2 = 0;

  //set to 1 when close button gets pressed
  int close_requested = 0;

  //animation loop
  while(!close_requested)
  {
    //processing events
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_QUIT:
          close_requested = 1;
          break;

        case SDL_KEYDOWN:
          switch(event.key.keysym.scancode)
          {
            case SDL_SCANCODE_W: up1=1; break;
            case SDL_SCANCODE_UP: up2=1; break;
            case SDL_SCANCODE_S: down1=1; break;
            case SDL_SCANCODE_DOWN: down2=1; break;
          } break;

        case SDL_KEYUP:
          switch(event.key.keysym.scancode)
          {
            case SDL_SCANCODE_W: up1=0; break;
            case SDL_SCANCODE_UP: up2=0; break;
            case SDL_SCANCODE_S: down1=0; break;
            case SDL_SCANCODE_DOWN: down2=0; break;
          } break;

        case SDL_MOUSEBUTTONDOWN:
          switch(event.button.button)
          {
            case SDL_BUTTON_LEFT:left=1;break;
            case SDL_BUTTON_RIGHT:right=1;break;
          } break;

        case SDL_MOUSEBUTTONUP:
          switch(event.button.button)
          {
            case SDL_BUTTON_LEFT: left=0;break;
            case SDL_BUTTON_RIGHT: right=0;break;
          } break;
      } break;
    }

    if(paddle1.y==0){up1=0;}
    if(paddle2.y==0){up2=0;}
    if(paddle1.y==WINDOW_HEIGHT-paddle1.h){down1=0;}
    if(paddle2.y==WINDOW_HEIGHT-paddle1.h){down2=0;}


    //computing paddle movement
    if(up1&&!down1){
      paddle1.y -= SPEED/30;
    }
    if(up2&&!down2){
      paddle2.y -= SPEED/30;
    }
    if(down1&&!up1){
      paddle1.y += SPEED/30;
    }
    if(down2&&!up2){
      paddle2.y += SPEED/30;
    }

    //computing ball movement
    if(left&&!ball_vx){ball_vx=ball_speed;}
    if(right&&!ball_vx){ball_vx=-ball_speed;}
    if(ball_vx!=0){
      ball.x += ball_vx*delay;
      ball.y += ball_vy*delay;
    }
    if(ball.x<0||ball.x>WINDOW_WIDTH-ball.w)
    {
      if(ball.x<0){++p2;}if(ball.x>WINDOW_WIDTH){++p1;}
      ball.y=(WINDOW_HEIGHT-ball.h)/2;ball.x=(WINDOW_WIDTH-ball.w)/2;
      ball_vx=0; ball_vy=0;
    }
    if(ball.y<0||ball.y>WINDOW_HEIGHT-ball.h){ball_vy=-ball_vy;}
    if(SDL_IntersectRect(&paddle1,&ball,&comp))
    {
      if(1){
      alpha=((comp.y-paddle1.y)/50.0 -1);
      ball_vx=cos(alpha)*ball_speed;
      ball_vy=sin(alpha)*ball_speed;}
      // else{
      //   ball_vx=-ball_vx;
      // }
    }
    else if(SDL_IntersectRect(&ball,&paddle2,&comp))
    {
      if(1){
      alpha=-((comp.y-paddle2.y)/50.0 -1);
      ball_vx=-cos(alpha)*ball_speed;
      ball_vy=sin(alpha)*ball_speed;}
      // else{
      //   ball_vx=-ball_vx;
      // }
    }
    //clear the window
    SDL_SetRenderDrawColor(rmain,0, 0, 0, 1);
    SDL_RenderClear(rmain);
    //updating image
    SDL_SetRenderDrawColor(rmain,10,160,10,1);
    SDL_RenderFillRect(rmain,&paddle1);
    SDL_RenderFillRect(rmain,&paddle2);
    SDL_RenderFillRect(rmain,&ball);
    SDL_RenderPresent(rmain);
    SDL_Delay(1000/60);
  }
  //Destroy everyhting reated and quit
  SDL_DestroyRenderer(rmain);
  SDL_DestroyWindow(wmain);
  SDL_Quit();
  printf("%d, %d",p1,p2);
  return 0;
}
