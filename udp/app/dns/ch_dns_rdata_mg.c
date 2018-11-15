/*
 *
 *      Filename: ch_dns_rdata_mg.c
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-05-03 10:23:52
 * Last Modified: 2018-05-09 16:39:14
 */

#include "ch_dns_rdata_mg.h"
#include "ch_log.h"

static void _rdata_mg_dump(ch_dns_rdata_t *rdata,FILE *fp,void *priv_data ch_unused){

	ch_dns_rdata_mg_t *mg = (ch_dns_rdata_mg_t*)rdata;

	fprintf(fp,"Dump The rdata.mg:\n");

	ch_dns_name_dump(&mg->name,fp);

}

static ssize_t  _rdata_mg_write(ch_dns_rdata_t *rdata,ch_data_output_t *dout,void *priv_data ch_unused){

	ch_dns_rdata_mg_t *mg = (ch_dns_rdata_mg_t*)rdata;

	return ch_dns_name_write(dout,&mg->name);

}

static ch_dns_rdata_t * _rdata_mg_create(ch_pool_t *mp,void *priv_data ch_unused){


	ch_dns_rdata_mg_t *rdata = (ch_dns_rdata_mg_t*)ch_pcalloc(mp,sizeof(ch_dns_rdata_mg_t));

	ch_dns_name_t *name = &rdata->name;

	rdata->rdata.rdata_dump = _rdata_mg_dump;
	rdata->rdata.rdata_write = _rdata_mg_write;

	CH_DNS_NAME_INIT(name);


	return (ch_dns_rdata_t*)rdata;

}

static int _rdata_mg_parse(ch_pool_t *mp,ch_dns_rdata_t *rdata,void *priv_data ch_unused){

	ch_dns_rdata_mg_t *rdata_mg = (ch_dns_rdata_mg_t*)rdata;
	
	if(rdata->dlen ==0 || rdata->data == NULL)
		return -1;

	if(ch_dns_rdata_name_parse(mp,rdata,&rdata_mg->name)){
	
		ch_log(CH_LOG_ERR,"Parse Rdata.mg failed!");
		return -1;
	}

	return 0;

}

static ch_dns_rdata_parser_t mg_parser = {

	{NULL,NULL},
	dns_rdataclass_in,
	dns_rdatatype_mg,
	_rdata_mg_create,
	_rdata_mg_parse
};


void ch_dns_rdata_mg_init(ch_dns_rdata_pool_t *rdata_pool){

	ch_dns_rdata_parser_register(rdata_pool,&mg_parser);

}

