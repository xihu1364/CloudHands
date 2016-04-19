/*
 * =====================================================================================
 *
 *       Filename:  ch_assemble.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年04月18日 14时52分56秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  shajf, csp001314@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "ch_log.h"
#include "ch_util.h"
#include "ch_assemble.h"
#include "ch_assemble_session.h"

static inline int _is_fin1_ack(ch_assemble_session_t *ass,struct tcp_hdr *th){

    uint32_t ack_seq = rte_be_to_cpu_32(th->recv_ack);

    return (th->tcp_flags&CH_TH_ACK)&&(ack_seq == ass->fin1_seq+1); 
}

static inline int _is_fin2_ack(ch_assemble_session_t *ass,struct tcp_hdr *th){

    uint32_t ack_seq = rte_be_to_cpu_32(th->recv_ack);

    return (th->tcp_flags&CH_TH_ACK)&&(ack_seq == ass->fin2_seq+1); 
}

static void assemble_session_flush(ch_assemble_task_t *astask,ch_assemble_session_t *ass){
    

}

static void assemble_session_close(ch_assemble_task_t *astask,ch_assemble_session_t *ass){

    /*flush session*/
    assemble_session_flush(astask,ass);

    /*free this assemble session */

    ch_assemble_session_pool_entry_free(astask->ass_pool,ass);

}

static void _process_fin_packet(ch_assemble_session_t *ass,struct tcp_hdr *th){

}

static void _process_rst_packet(ch_assemble_session_t *ass,struct tcp_hdr *th){

}

static void _process_data_packet(ch_assemble_session_t *ass,struct tcp_hdr *th){

}

static void _process_fin1_ack_packet(ch_assemble_session_t *ass,struct tcp_hdr *th){

}

static void _process_fin2_ack_packet(ch_assemble_session_t *ass,struct tcp_hdr *th){

}

void ch_assemble_do(ch_assemble_task_t *astask,struct rte_mbuf *mbuf){

    ch_four_tuple_t tuple;
    ch_assemble_session_t *ass;
    ch_assemble_session_endpoint_t *ep;

    struct ipv4_hdr *iph = ch_ipv4_hdr_get(mbuf); 
    struct tcp_hdr *th = ch_tcp_hdr_get(mbuf);
    void *pl_data = NULL;
    size_t pl_len = 0;

    ch_four_tuple_init2(&tuple,iph,th);

    do{
        /*find the assemble session*/
        ass = ch_assemble_session_pool_entry_find(astask->ass_pool,&tuple);
        if(ass == NULL){
            ch_log(CH_LOG_ERR,"The assemble session no existed !");
            break;
        }
        /*find the assemble session's endpoint*/
        ep = ch_assemble_session_endpoint_get(ass,&tuple);
        if(ep == NULL){
            ch_log(CH_LOG_ERR,"Cannot get the assemble session endpoint!");
            break;
        }
        ass->cur_ep = ep;

        /*fin packet*/
        if(is_tcp_fin_packet(th)){
            _process_fin_packet(ass,th);
            break;
        }

        /*rest packet*/
        if(is_tcp_rst_packet(th)){
            _process_rst_packet(ass,th);
            break;
        }

        /*data packet*/
        ch_packet_data_payload_get(mbuf,&pl_data,&pl_len);
        if(pl_len){
           _process_data_packet(ass,th); 
        }

        /*fin ack packet!*/
        if(_is_fin1_ack(ass,th)){
            _process_fin1_ack_packet(ass,th);
            break;
        }

        if(_is_fin2_ack(ass,th)){
            _process_fin2_ack_packet(ass,th);
            break;
        }

    }while(0);

    /*free mbuf!*/
    rte_pktmbuf_free(mbuf);
}

