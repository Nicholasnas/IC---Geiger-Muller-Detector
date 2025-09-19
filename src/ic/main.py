import serial
from datetime import datetime
import time



class GeigerCounter:
    
    def __init__(self, arquivo:str = 'dados_geiger_sem_camisinha.csv'):
        self.port_serial = "/dev/ttyACM0" 
        self.conn_vel = 9600         
        self.file = arquivo
        self.conn = self.connection()
        
    def connection(self):
        try:
            conn = serial.Serial(self.port_serial, self.conn_vel)
            time.sleep(2) # Arduino finalizar o reset
            print("Ok, conectado.")
            return conn

        except serial.SerialException as error:
            print(f"Erro ao abrir a porta serial: {error}")
            exit()
    
    def reading_conn_serial(self):
                
        with open(self.file, "a", encoding="utf-8") as file:
            file.write("timestamp,CPS,CPM,uSv/h,mR/h\n")  

            try:
                while True:
                    # Pega a linha completa da porta serial
                    linha = self.conn.readline().decode("utf-8").strip()
                    
                    dados = linha.split(',')
                    cps = dados[0]
                    cpm = dados[1]
                    uSv = dados[2]
                    mREM = dados[3]
                    
                    agora = datetime.now().strftime(r"%Y-%m-%d %H:%M:%S")

                    # Motando os dados no formato csv
                    dados_csv = f"{agora},{cps},{cpm},{uSv},{mREM}\n"

                    # Salva no arquivo e forca a escrita no disco.      
                    file.write(dados_csv)
                    file.flush()  
                    
                    print(f"Timestamp: {agora}, CPS: {cps}, CPM: {cpm}, uSv/h: {uSv}, mR/h: {mREM}")
                    
            except KeyboardInterrupt:
                print("Coleta de dados encerrada pelo usuario") # ctrl + c
            except Exception as error:
                # Captura outros erros inesperados.
                print(f"Ocorreu um erro: {error}")
            finally:
                # Fechando a conexao serial
                self.conn.close()
                print("Fim")
                
                

if __name__ == "__main__":
    geiger = GeigerCounter()
    geiger.reading_conn_serial()
    