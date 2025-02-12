#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <rte_eal.h>
#include <rte_mbuf.h>
#include <rte_ethdev.h>

#define RX_RING_SIZE 1024
#define NUM_MBUFS 8192
#define MBUF_CACHE_SIZE 250
#define BURST_SIZE 32

// Função para printar o payload em HEX e ASCII
void print_payload(uint8_t *data, uint16_t len) {
    printf("Payload (HEX & ASCII):\n");
    for (uint16_t i = 0; i < len; i++) {
        printf("%02X ", data[i]); // HEX
        if ((i + 1) % 16 == 0 || i == len - 1) { // Exibir ASCII ao lado
            printf("  ");
            for (uint16_t j = (i / 16) * 16; j <= i; j++) {
                printf("%c", (data[j] >= 32 && data[j] <= 126) ? data[j] : '.');
            }
            printf("\n");
        }
    }
}

int main(int argc, char *argv[]) {
    struct rte_mempool *mbuf_pool;
    unsigned nb_ports, port_id;

    // Inicializar o DPDK
    if (rte_eal_init(argc, argv) < 0)
        rte_exit(EXIT_FAILURE, "Erro ao inicializar o EAL\n");

    nb_ports = rte_eth_dev_count_avail();
    if (nb_ports == 0)
        rte_exit(EXIT_FAILURE, "Nenhuma NIC disponível\n");

    printf("Detectei %u portas de rede\n", nb_ports);

    port_id = 0; // Captura da porta 0 (pode mudar para outra)

    // Criar um mempool para armazenar pacotes
    mbuf_pool = rte_pktmbuf_pool_create("MBUF_POOL", NUM_MBUFS,
                                        MBUF_CACHE_SIZE, 0, RTE_MBUF_DEFAULT_BUF_SIZE,
                                        rte_socket_id());
    if (mbuf_pool == NULL)
        rte_exit(EXIT_FAILURE, "Erro ao criar mempool\n");

    // Configurar a porta de rede
    struct rte_eth_conf port_conf = {0};
    if (rte_eth_dev_configure(port_id, 1, 1, &port_conf) < 0)
        rte_exit(EXIT_FAILURE, "Erro ao configurar porta\n");

    // Criar fila de recepção
    if (rte_eth_rx_queue_setup(port_id, 0, RX_RING_SIZE, rte_eth_dev_socket_id(port_id),
                               NULL, mbuf_pool) < 0)
        rte_exit(EXIT_FAILURE, "Erro ao configurar fila de RX\n");

    // Iniciar a porta
    if (rte_eth_dev_start(port_id) < 0)
        rte_exit(EXIT_FAILURE, "Erro ao iniciar a porta\n");

    printf("Capturando pacotes na porta %u...\n", port_id);

    struct rte_mbuf *bufs[BURST_SIZE];
    while (1) {
        // Receber pacotes
        uint16_t nb_rx = rte_eth_rx_burst(port_id, 0, bufs, BURST_SIZE);
        if (nb_rx == 0)
            continue;

        // Processar cada pacote recebido
        for (uint16_t i = 0; i < nb_rx; i++) {
            struct rte_mbuf *mbuf = bufs[i];

            uint8_t *payload = rte_pktmbuf_mtod(mbuf, uint8_t *);
            uint16_t payload_len = rte_pktmbuf_pkt_len(mbuf);

            printf("\n=== Pacote recebido (%u bytes) ===\n", payload_len);
            print_payload(payload, payload_len);

            // Liberar o pacote
            rte_pktmbuf_free(mbuf);
        }
    }

    return 0;
}
