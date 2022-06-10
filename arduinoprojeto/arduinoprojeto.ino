#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int senha = 0;              //Senha do cliente 
int senha_atual = 0;        //Senha do monitor 
int senha_antiga;           //Senha anterior a atual
int pessoas_frente;         //Quantidade de pessoas que estão a frente
int cont = 1;               //Contador de usos da comanda
int set = 0;                //Controle da configuração

int SetSenha_atual()
{
    if(Serial.available())
    {
        senha_atual = Serial.parseInt();
        return 1;
    }
    return 0;
}

int SetSenha()
{
    if(Serial.available())
    {
        senha = Serial.parseInt();
        return 1;
    }
    return 0;
}

void TocarComanda(int del) //Função que faz a comanda tocar
{
    tone(10, 196, del);
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(9, HIGH);
    delay(del);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(9, LOW);
    delay(del);
}

void setup() 
{
    Serial.begin(9600);
    Serial.setTimeout(10000000);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(9, OUTPUT);  
    lcd.begin(16, 2);
}

void loop() 
{
    if(!set) //Configuração das senhas
    {
        lcd.clear();
        lcd.print("Configurando...");

        Serial.println("");
        Serial.println("Digite a senha atual: ");
        while(!SetSenha_atual());
        Serial.println(senha_atual);

        Serial.println("Digite a senha do Cliente: ");
        while(!SetSenha());
        Serial.println(senha);
        
        Serial.println("Senha atual: ");
        Serial.print(senha_atual);
        Serial.write(" | ");
        set = 1;
        pessoas_frente = senha - senha_atual;
        lcd.clear();
        lcd.print("Pessoas frente:");
        lcd.setCursor(0, 1);
        lcd.print(pessoas_frente);
    }
    else
    {
        char tecla;
        tecla = Serial.read();

        senha_antiga = senha_atual;
        
        if (tecla == '-') //Avançar fila
        {
            senha_antiga = senha_atual;
            senha_atual = senha_atual + 1;
            pessoas_frente = senha - senha_atual;
            lcd.clear();
            lcd.print("Pessoas frente:");
            lcd.setCursor(0, 1);
            lcd.print(pessoas_frente);
        }

        if (tecla == '+') //Recuar fila
        {
            senha_antiga = senha_atual;
            senha_atual = senha_atual - 1;
            pessoas_frente = senha - senha_atual;
            lcd.clear();
            lcd.print("Pessoas frente:");
            lcd.setCursor(0, 1);
            lcd.print(pessoas_frente);
        }

        if (tecla == 'x') //Reiniciar a fila
        {
            lcd.clear();
            lcd.print("Reiniciando!");
            cont = cont + 1;
            set = 0;
            lcd.setCursor(0, 1);
            lcd.print(".");
            delay(300);
            lcd.print(" .");
            delay(300);
            lcd.print(" .");
            delay(300);
        }

        if (tecla == 'r') //Exibir relatório no Monitor Serial 
        {
            long tempo = millis() / 1000;
            Serial.println("");
            Serial.println("---------- Relatorio ----------");
            Serial.println("Segundos que a comanda esta ligada: ");
            Serial.println(tempo);
            Serial.println("Vezes que a comanda foi usada: ");
            Serial.println(cont);
            Serial.println("Media de tempo na fila: ");
            Serial.println(tempo / cont);
            Serial.println("-------------------------------");
        }

        if (senha_antiga != senha_atual) //Tocar quando a fila andar
        {
            Serial.print(senha_atual);
            Serial.write(" | ");
            if (pessoas_frente > 3)
                TocarComanda(200);
            else if (pessoas_frente > 0)
            {
                for (int i = 0; i < pessoas_frente; i++)
                    TocarComanda(500);
            }
            else
            {
                lcd.clear();
                lcd.print("Chegou a");
                lcd.setCursor(0, 1);
                lcd.print("sua vez!");
                int del = 600;
                for (int i = 0; i < 6; i++)
                {
                    TocarComanda(del);
                    del = del - 100;
                }
            }
        }
    }    
}
