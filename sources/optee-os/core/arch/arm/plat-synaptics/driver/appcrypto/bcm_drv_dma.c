



#include "bcm_drv_dma.h"
#include "bcm_drv_spad.h"
#include "bcm_drv_xbar.h"


void bcm_drv_dma_init()
{
    volatile BCM_DMA *dma = (BCM_DMA *) BASE_DMA;

    //Config the DMA IN/OUT config registers
    dma->output_dma_control_register &= DMA_CONTROL_REG_CLEAR;
    dma->output_dma_control_register |= DMA_CONTROL_REG_DEFAULT;
    dma->input_dma_control_register &= DMA_CONTROL_REG_CLEAR;
    dma->input_dma_control_register |= DMA_CONTROL_REG_DEFAULT;
}


void bcm_drv_dma_setup_link_list(uint32_t direction,
        uint32_t xfer_adr, uint32_t xfer_cnt, uint32_t nx_ll_adr, uint32_t *ref_count)
{
    volatile BCM_DMA *dma = (BCM_DMA *) BASE_DMA;
    *ref_count = 0;

    if(direction)
    {
        dma->dma_output_fifo_destination_address=xfer_adr;
        dma->output_dma_transfer_counter=xfer_cnt;
        dma->output_dma_next_link_list_address=nx_ll_adr;
        *ref_count = dma->output_dma_transfer_counter;
    }
    else
    {
        dma->dma_source_address=xfer_adr;
        dma->dma_input_transfer_counter=xfer_cnt;
        dma->input_dma_next_link_list_address=nx_ll_adr;
        *ref_count = dma->dma_input_transfer_counter;
    }
}

STATUS bcm_drv_dma_wait_4_done(uint32_t direction, uint32_t time_out)
{
    volatile BCM_DMA *dma = (BCM_DMA *) BASE_DMA;
    volatile BCM_APB_ADEC *apb_adec= (BCM_APB_ADEC *) BASE_APB_ADEC;
    volatile uint32_t temp = 0;
    while(time_out--)
    {
        //read interrupt register
        if(direction)
            temp=dma->output_dma_interrupt_1;
        else
            temp=dma->input_dma_interrupt_1;
    
        //clear and check interrupt
        //[5:1] = error interrupts
        if(temp&0x3F)
        {
            //disable dma and clear interrupt
            if(direction)
            {
                dma->output_dma_control_register&=0xfffffffe;
                dma->output_dma_interrupt_1=temp;
                apb_adec->address_decoder_interrupt_register=0x20;//ENG5
            }
            else
            {
                dma->input_dma_control_register &=0xfffffffe;
                dma->input_dma_interrupt_1=temp;
                apb_adec->address_decoder_interrupt_register=0x20;//ENG5
            }
        
            //check error bits
            if (temp & 0x02) return (STATUS_DMA_BUS_ERROR);
            if (temp & 0x04) return (STATUS_DMA_LINKED_LIST_ACCESS_ERROR);
            if (temp & 0x08) return (STATUS_DMA_IDIOPATHIC_ERROR); //FIFO_PARITY_ERROR
            if (temp & 0x10) return (STATUS_DMA_PAUSE_COMPLETION_TIMEOUT);
            if (temp & 0x20) return (STATUS_DMA_PARITY_ERROR);
            return (STATUS_SUCCESS); 
        }
    }  
    return (STATUS_DMA_TIMEOUT); 
}


STATUS bcm_drv_dma_in_sp(uint32_t soc_src_addr, uint32_t size_word, uint32_t sp_wr_offset)
{
    STATUS status = STATUS_SUCCESS;
    uint32_t ref_size = 0;

    if(size_word > BCM_V3_SPAD_SIZE_IN_WORD)
        return STATUS_FAILURE;

    if(size_word == 0) return STATUS_SUCCESS;


    bcm_drv_xbar_set_path(IN_DMA_PATH);

    bcm_drv_spad_set_access_mode(SPAD_SEQUENTIAL_ACCESS);
    bcm_drv_spad_set_wr_ptr(sp_wr_offset);
    bcm_drv_spad_set_rd_ptr(sp_wr_offset);

    bcm_drv_dma_setup_link_list(0, soc_src_addr, size_word, 0, &ref_size);
    security_assert(ref_size == size_word);

    bcm_drv_dma_enable(0);

    status =  bcm_drv_dma_wait_4_done(0, 10000000);

    return status;
}

STATUS bcm_drv_dma_out_sp(uint32_t soc_dst_addr, uint32_t size_word, uint32_t sp_rd_offset)
{
    STATUS status = STATUS_SUCCESS;
    uint32_t ref_size = 0;

    if(size_word > BCM_V3_SPAD_SIZE_IN_WORD)
        return STATUS_FAILURE;

    if(size_word == 0) return STATUS_SUCCESS;


    bcm_drv_xbar_set_path(OUT_DMA_PATH);

    bcm_drv_spad_set_access_mode(SPAD_SEQUENTIAL_ACCESS);

    bcm_drv_spad_set_wr_ptr(sp_rd_offset + size_word);
    bcm_drv_spad_set_rd_ptr(sp_rd_offset);

    bcm_drv_dma_setup_link_list(1, soc_dst_addr, size_word, 0, &ref_size);
    security_assert(ref_size == size_word);

    bcm_drv_dma_enable(1);

    status =  bcm_drv_dma_wait_4_done(1, 10000000);

    return status;
}


STATUS bcm_drv_dma_in(uint32_t soc_src_addr, uint32_t size_word, uint32_t* local_buf)
{
    STATUS status = STATUS_SUCCESS;
    uint32_t round = 0;
    uint32_t residual = 0;
    uint32_t i = 0;
    uint32_t ref_size = 0;

    round = size_word/BCM_V3_SPAD_SIZE_IN_WORD;
    residual = size_word%BCM_V3_SPAD_SIZE_IN_WORD;

    status = bcm_drv_dma_in_sp(soc_src_addr, residual, 0);
    if(status)
        return status;
    bcm_drv_spad_read(local_buf, residual, 0, &ref_size);
    security_assert(ref_size == residual);

    for(i=0;i<round;i++)
    {
        ref_size = 0;
        status = bcm_drv_dma_in_sp(soc_src_addr + (residual + i*BCM_V3_SPAD_SIZE_IN_WORD)*sizeof(uint32_t),
                               BCM_V3_SPAD_SIZE_IN_WORD,
                               0);
        if(status) return status;

        bcm_drv_spad_read(local_buf+residual+i*BCM_V3_SPAD_SIZE_IN_WORD,
                          BCM_V3_SPAD_SIZE_IN_WORD,
                          0, &ref_size);
    security_assert(ref_size == BCM_V3_SPAD_SIZE_IN_WORD);
    }
    return status;
}


STATUS bcm_drv_dma_out(uint32_t soc_dst_addr, uint32_t size_word, uint32_t* local_buf)
{

    STATUS status = STATUS_SUCCESS;
    uint32_t round = 0;
    uint32_t residual = 0;
    uint32_t i = 0;

    round = size_word/BCM_V3_SPAD_SIZE_IN_WORD;
    residual = size_word%BCM_V3_SPAD_SIZE_IN_WORD;

    bcm_drv_spad_set_rd_ptr(0);
    bcm_drv_spad_set_wr_ptr(0);
    bcm_drv_spad_write(local_buf, residual, 0);
    status = bcm_drv_dma_out_sp(soc_dst_addr, residual, 0);
    if(status) return status;

    for(i=0;i<round;i++)
    {
        bcm_drv_spad_write(local_buf+residual+i*BCM_V3_SPAD_SIZE_IN_WORD,
                          BCM_V3_SPAD_SIZE_IN_WORD,
                          0);
        status = bcm_drv_dma_out_sp(soc_dst_addr + (residual + i*BCM_V3_SPAD_SIZE_IN_WORD)*sizeof(uint32_t),
                               BCM_V3_SPAD_SIZE_IN_WORD,
                               0);
        if(status) return status;

    }
    return status;
}

STATUS bcm_drv_dma_in_out(uint32_t soc_dst, uint32_t soc_src, uint32_t size_byte)
{
    STATUS status = STATUS_SUCCESS;
    uint32_t ref_size = 0;

    bcm_drv_dma_init();

    bcm_drv_xbar_set_path(IN_OUT_DMA_PATH);
    bcm_drv_spad_set_access_mode(SPAD_SEQUENTIAL_ACCESS);

    // set source address to read
    bcm_drv_dma_setup_link_list(0x0, (uint32_t)soc_src, size_byte/sizeof(uint32_t), 0, &ref_size);
    security_assert(ref_size == size_byte/sizeof(uint32_t));
    ref_size = 0;

    bcm_drv_dma_setup_link_list(0x1, (uint32_t)soc_dst, size_byte/sizeof(uint32_t), 0, &ref_size);
    security_assert(ref_size == size_byte/sizeof(uint32_t));

    //kick off dma
    bcm_drv_dma_enable(0x0);
    bcm_drv_dma_enable(0x1);


    status = bcm_drv_dma_wait_4_done(0, DMA_TIMEOUT);
    if(status) return status;

    status = bcm_drv_dma_wait_4_done(1, DMA_TIMEOUT);

    return status;
}
