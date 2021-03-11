/*
 * Copyright (c) 2020 Demant
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <ztest.h>
#include "kconfig.h"

#define ULL_LLCP_UNITTEST

#include <bluetooth/hci.h>
#include <sys/byteorder.h>
#include <sys/slist.h>
#include <sys/util.h>
#include "hal/ccm.h"

#include "util/mem.h"
#include "util/memq.h"

#include "pdu.h"
#include "ll.h"
#include "ll_settings.h"

#include "lll.h"
#include "lll_conn.h"

#include "ull_tx_queue.h"

#include "ull_conn_types.h"
#include "ull_llcp.h"
#include "ull_llcp_internal.h"

#include "helper_pdu.h"
#include "helper_util.h"

static struct ll_conn conn;

static void setup(void)
{
	test_setup(&conn);
}

static bool is_instant_reached(struct ll_conn *conn, uint16_t instant)
{
	return ((event_counter(conn) - instant) & 0xFFFF) <= 0x7FFF;
}

/* +-----+                +-------+              +-----+
 * | UT  |                | LL_A  |              | LT  |
 * +-----+                +-------+              +-----+
 *    |                       |                     |
 */
void test_phy_update_mas_loc(void)
{
	uint8_t err;
	struct node_tx *tx;
	struct node_rx_pdu *ntf;
	struct pdu_data *pdu;
	uint16_t instant;

	struct node_rx_pu pu = {
		.status = BT_HCI_ERR_SUCCESS
	};

	/* Role */
	test_set_role(&conn, BT_HCI_ROLE_MASTER);

	/* Connect */
	ull_cp_state_set(&conn, ULL_CP_CONNECTED);

	/* Initiate an PHY Update Procedure */
	err = ull_cp_phy_update(&conn);
	zassert_equal(err, BT_HCI_ERR_SUCCESS, NULL);

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should have one LL Control PDU */
	lt_rx(LL_PHY_REQ, &conn, &tx, NULL);
	lt_rx_q_is_empty(&conn);

	/* Rx */
	lt_tx(LL_PHY_RSP, &conn, NULL);

	/* Done */
	event_done(&conn);

	/* Release Tx */
	ull_cp_release_tx(tx);

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should have one LL Control PDU */
	lt_rx(LL_PHY_UPDATE_IND, &conn, &tx, NULL);
	lt_rx_q_is_empty(&conn);

	/* Done */
	event_done(&conn);

	/* Save Instant */
	pdu = (struct pdu_data *)tx->pdu;
	instant = sys_le16_to_cpu(pdu->llctrl.phy_upd_ind.instant);

	/* Release Tx */
	ull_cp_release_tx(tx);

	/* */
	while (!is_instant_reached(&conn, instant)) {
		/* Prepare */
		event_prepare(&conn);

		/* Tx Queue should NOT have a LL Control PDU */
		lt_rx_q_is_empty(&conn);

		/* Done */
		event_done(&conn);

		/* There should NOT be a host notification */
		ut_rx_q_is_empty();
	}

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should NOT have a LL Control PDU */
	lt_rx_q_is_empty(&conn);

	/* Done */
	event_done(&conn);

	/* There should be one host notification */
	ut_rx_node(NODE_PHY_UPDATE, &ntf, &pu);
	ut_rx_q_is_empty();

	/* Release Ntf */
	ull_cp_release_ntf(ntf);

	zassert_equal(ctx_buffers_free(), PROC_CTX_BUF_NUM, "Free CTX buffers %d", ctx_buffers_free());
}

void test_phy_update_mas_loc_unsupp_feat(void)
{
	uint8_t err;
	struct node_tx *tx;
	struct node_rx_pdu *ntf;

	struct pdu_data_llctrl_unknown_rsp unknown_rsp = {
		.type = PDU_DATA_LLCTRL_TYPE_PHY_REQ
	};

	struct node_rx_pu pu = {
		.status = BT_HCI_ERR_UNSUPP_REMOTE_FEATURE
	};

	/* Role */
	test_set_role(&conn, BT_HCI_ROLE_MASTER);

	/* Connect */
	ull_cp_state_set(&conn, ULL_CP_CONNECTED);

	/* Initiate an PHY Update Procedure */
	err = ull_cp_phy_update(&conn);
	zassert_equal(err, BT_HCI_ERR_SUCCESS, NULL);

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should have one LL Control PDU */
	lt_rx(LL_PHY_REQ, &conn, &tx, NULL);
	lt_rx_q_is_empty(&conn);

	/* Rx */
	lt_tx(LL_UNKNOWN_RSP, &conn, &unknown_rsp);

	/* Done */
	event_done(&conn);

	/* Release Tx */
	ull_cp_release_tx(tx);

	/* There should be one host notification */
	ut_rx_node(NODE_PHY_UPDATE, &ntf, &pu);
	ut_rx_q_is_empty();

	/* Release Ntf */
	ull_cp_release_ntf(ntf);

	zassert_equal(ctx_buffers_free(), PROC_CTX_BUF_NUM, "Free CTX buffers %d", ctx_buffers_free());
}

void test_phy_update_mas_rem(void)
{
	struct node_tx *tx;
	struct node_rx_pdu *ntf;
	struct pdu_data *pdu;
	uint16_t instant;

	struct node_rx_pu pu = {
		.status = BT_HCI_ERR_SUCCESS
	};

	/* Role */
	test_set_role(&conn, BT_HCI_ROLE_MASTER);

	/* Connect */
	ull_cp_state_set(&conn, ULL_CP_CONNECTED);

	/* Prepare */
	event_prepare(&conn);

	/* Rx */
	lt_tx(LL_PHY_REQ, &conn, NULL);

	/* Done */
	event_done(&conn);

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should have one LL Control PDU */
	lt_rx(LL_PHY_UPDATE_IND, &conn, &tx, NULL);
	lt_rx_q_is_empty(&conn);

	/* Done */
	event_done(&conn);

	/* Save Instant */
	pdu = (struct pdu_data *)tx->pdu;
	instant = sys_le16_to_cpu(pdu->llctrl.phy_upd_ind.instant);

	/* Release Tx */
	ull_cp_release_tx(tx);

	/* */
	while (!is_instant_reached(&conn, instant)) {
		/* Prepare */
		event_prepare(&conn);

		/* Tx Queue should NOT have a LL Control PDU */
		lt_rx_q_is_empty(&conn);

		/* Done */
		event_done(&conn);

		/* There should NOT be a host notification */
		ut_rx_q_is_empty();
	}

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should NOT have a LL Control PDU */
	lt_rx_q_is_empty(&conn);

	/* Done */
	event_done(&conn);

	/* There should be one host notification */
	ut_rx_node(NODE_PHY_UPDATE, &ntf, &pu);
	ut_rx_q_is_empty();

	/* Release Ntf */
	ull_cp_release_ntf(ntf);

	zassert_equal(ctx_buffers_free(), PROC_CTX_BUF_NUM, "Free CTX buffers %d", ctx_buffers_free());
}

void test_phy_update_sla_loc(void)
{
	uint8_t err;
	struct node_tx *tx;
	struct node_rx_pdu *ntf;
	uint16_t instant;

	struct node_rx_pu pu = {
		.status = BT_HCI_ERR_SUCCESS
	};

	struct pdu_data_llctrl_phy_upd_ind phy_update_ind = {0};

	/* Role */
	test_set_role(&conn, BT_HCI_ROLE_SLAVE);

	/* Connect */
	ull_cp_state_set(&conn, ULL_CP_CONNECTED);

	/* Initiate an PHY Update Procedure */
	err = ull_cp_phy_update(&conn);
	zassert_equal(err, BT_HCI_ERR_SUCCESS, NULL);

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should have one LL Control PDU */
	lt_rx(LL_PHY_REQ, &conn, &tx, NULL);
	lt_rx_q_is_empty(&conn);

	/* Done */
	event_done(&conn);

	/* Release Tx */
	ull_cp_release_tx(tx);

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should NOT have a LL Control PDU */
	lt_rx_q_is_empty(&conn);

	/* Rx */
	phy_update_ind.instant = instant = event_counter(&conn) + 6;
	lt_tx(LL_PHY_UPDATE_IND, &conn, &phy_update_ind);

	/* Done */
	event_done(&conn);

	/* */
	while (!is_instant_reached(&conn, instant)) {
		/* Prepare */
		event_prepare(&conn);

		/* Tx Queue should NOT have a LL Control PDU */
		lt_rx_q_is_empty(&conn);

		/* Done */
		event_done(&conn);

		/* There should NOT be a host notification */
		ut_rx_q_is_empty();
	}

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should NOT have a LL Control PDU */
	lt_rx_q_is_empty(&conn);

	/* Done */
	event_done(&conn);

	/* There should be one host notification */
	ut_rx_node(NODE_PHY_UPDATE, &ntf, &pu);
	ut_rx_q_is_empty();

	/* Release Ntf */
	ull_cp_release_ntf(ntf);

	zassert_equal(ctx_buffers_free(), PROC_CTX_BUF_NUM, "Free CTX buffers %d", ctx_buffers_free());
}

void test_phy_update_sla_rem(void)
{
	struct node_tx *tx;
	struct node_rx_pdu *ntf;
	uint16_t instant;

	struct node_rx_pu pu = {
		.status = BT_HCI_ERR_SUCCESS
	};

	struct pdu_data_llctrl_phy_upd_ind phy_update_ind = {0};

	/* Role */
	test_set_role(&conn, BT_HCI_ROLE_SLAVE);

	/* Connect */
	ull_cp_state_set(&conn, ULL_CP_CONNECTED);

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should NOT have a LL Control PDU */
	lt_rx_q_is_empty(&conn);

	/* Rx */
	lt_tx(LL_PHY_REQ, &conn, NULL);

	/* Done */
	event_done(&conn);

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should have one LL Control PDU */
	lt_rx(LL_PHY_RSP, &conn, &tx, NULL);
	lt_rx_q_is_empty(&conn);

	/* Rx */
	phy_update_ind.instant = instant = event_counter(&conn) + 6;
	lt_tx(LL_PHY_UPDATE_IND, &conn, &phy_update_ind);

	/* Done */
	event_done(&conn);

	/* Release Tx */
	ull_cp_release_tx(tx);

	/* */
	while (!is_instant_reached(&conn, instant)) {
		/* Prepare */
		event_prepare(&conn);

		/* Tx Queue should NOT have a LL Control PDU */
		lt_rx_q_is_empty(&conn);

		/* Done */
		event_done(&conn);

		/* There should NOT be a host notification */
		ut_rx_q_is_empty();
	}

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should NOT have a LL Control PDU */
	lt_rx_q_is_empty(&conn);

	/* Done */
	event_done(&conn);

	/* There should be one host notification */
	ut_rx_node(NODE_PHY_UPDATE, &ntf, &pu);
	ut_rx_q_is_empty();

	/* Release Ntf */
	ull_cp_release_ntf(ntf);

	zassert_equal(ctx_buffers_free(), PROC_CTX_BUF_NUM, "Free CTX buffers %d", ctx_buffers_free());
}

void test_phy_update_mas_loc_collision(void)
{
	uint8_t err;
	struct node_tx *tx;
	struct node_rx_pdu *ntf;
	struct pdu_data *pdu;
	uint16_t instant;

	struct pdu_data_llctrl_reject_ext_ind reject_ext_ind = {
		.reject_opcode = PDU_DATA_LLCTRL_TYPE_PHY_REQ,
		.error_code = BT_HCI_ERR_LL_PROC_COLLISION
	};

	struct node_rx_pu pu = {
		.status = BT_HCI_ERR_SUCCESS
	};

	/* Role */
	test_set_role(&conn, BT_HCI_ROLE_MASTER);

	/* Connect */
	ull_cp_state_set(&conn, ULL_CP_CONNECTED);

	/* Initiate an PHY Update Procedure */
	err = ull_cp_phy_update(&conn);
	zassert_equal(err, BT_HCI_ERR_SUCCESS, NULL);

	/*** ***/

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should have one LL Control PDU */
	lt_rx(LL_PHY_REQ, &conn, &tx, NULL);
	lt_rx_q_is_empty(&conn);

	/* Rx */
	lt_tx(LL_PHY_REQ, &conn, NULL);

	/* Done */
	event_done(&conn);

	/* Release Tx */
	ull_cp_release_tx(tx);

	/*** ***/

	/* Prepare */
	event_prepare(&conn);
	test_print_conn(&conn);
	/* Tx Queue should have one LL Control PDU */
	printf("Tx REJECT\n");
	lt_rx(LL_REJECT_EXT_IND, &conn, &tx, &reject_ext_ind);
	lt_rx_q_is_empty(&conn);

	/* Done */
	printf("Done again\n");
	event_done(&conn);

	/* Release Tx */
	ull_cp_release_tx(tx);

	/*** ***/

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should NOT have a LL Control PDU */
	printf("Empty\n");
	lt_rx_q_is_empty(&conn);

	/* Rx */
	printf("Tx again\n");
	lt_tx(LL_PHY_RSP, &conn, NULL);

	/* Done */
	event_done(&conn);

	/*** ***/

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should have one LL Control PDU */
	printf("And again\n");
	lt_rx(LL_PHY_UPDATE_IND, &conn, &tx, NULL);
	lt_rx_q_is_empty(&conn);

	/* Done */
	event_done(&conn);

	/* Save Instant */
	pdu = (struct pdu_data *)tx->pdu;
	instant = sys_le16_to_cpu(pdu->llctrl.phy_upd_ind.instant);

	/* Release Tx */
	ull_cp_release_tx(tx);

	/* */
	while (!is_instant_reached(&conn, instant)) {
		/* Prepare */
		event_prepare(&conn);

		/* Tx Queue should NOT have a LL Control PDU */
		lt_rx_q_is_empty(&conn);

		/* Done */
		event_done(&conn);

		/* There should NOT be a host notification */
		ut_rx_q_is_empty();
	}

	/*** ***/

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should NOT have a LL Control PDU */
	lt_rx_q_is_empty(&conn);

	/* Done */
	event_done(&conn);

	/* There should be one host notification */
	ut_rx_node(NODE_PHY_UPDATE, &ntf, &pu);
	ut_rx_q_is_empty();

	/* Release Ntf */
	ull_cp_release_ntf(ntf);

	zassert_equal(ctx_buffers_free(), PROC_CTX_BUF_NUM, "Free CTX buffers %d", ctx_buffers_free());
}

void test_phy_update_mas_rem_collision(void)
{
	uint8_t err;
	struct node_tx *tx;
	struct node_rx_pdu *ntf;
	struct pdu_data *pdu;
	uint16_t instant;

	struct node_rx_pu pu = {
		.status = BT_HCI_ERR_SUCCESS
	};

	/* Role */
	test_set_role(&conn, BT_HCI_ROLE_MASTER);

	/* Connect */
	ull_cp_state_set(&conn, ULL_CP_CONNECTED);

	/*** ***/

	/* Prepare */
	event_prepare(&conn);

	/* Rx */
	lt_tx(LL_PHY_REQ, &conn, NULL);

	/* Done */
	event_done(&conn);

	/*** ***/

	/* Initiate an PHY Update Procedure */
	err = ull_cp_phy_update(&conn);
	zassert_equal(err, BT_HCI_ERR_SUCCESS, NULL);

	/*** ***/

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should have one LL Control PDU */
	lt_rx(LL_PHY_UPDATE_IND, &conn, &tx, NULL);
	lt_rx_q_is_empty(&conn);

	/* Done */
	event_done(&conn);

	/* Save Instant */
	pdu = (struct pdu_data *)tx->pdu;
	instant = sys_le16_to_cpu(pdu->llctrl.phy_upd_ind.instant);

	/* Release Tx */
	ull_cp_release_tx(tx);

	/* */
	while (!is_instant_reached(&conn, instant)) {
		/* Prepare */
		event_prepare(&conn);

		/* Tx Queue should NOT have a LL Control PDU */
		lt_rx_q_is_empty(&conn);

		/* Done */
		event_done(&conn);

		/* There should NOT be a host notification */
		ut_rx_q_is_empty();
	}

	/*** ***/

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should have one LL Control PDU */
	lt_rx(LL_PHY_REQ, &conn, &tx, NULL);
	lt_rx_q_is_empty(&conn);

	/* Rx */
	lt_tx(LL_PHY_RSP, &conn, NULL);

	/* Done */
	event_done(&conn);

	/* Release Tx */
	ull_cp_release_tx(tx);

	/* There should be one host notification */
	ut_rx_node(NODE_PHY_UPDATE, &ntf, &pu);
	ut_rx_q_is_empty();

	/* Release Ntf */
	ull_cp_release_ntf(ntf);

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should have one LL Control PDU */
	lt_rx(LL_PHY_UPDATE_IND, &conn, &tx, NULL);
	lt_rx_q_is_empty(&conn);

	/* Done */
	event_done(&conn);

	/* Save Instant */
	pdu = (struct pdu_data *)tx->pdu;
	instant = sys_le16_to_cpu(pdu->llctrl.phy_upd_ind.instant);

	/* Release Tx */
	ull_cp_release_tx(tx);

	/* */
	while (!is_instant_reached(&conn, instant)) {
		/* Prepare */
		event_prepare(&conn);

		/* Tx Queue should NOT have a LL Control PDU */
		lt_rx_q_is_empty(&conn);

		/* Done */
		event_done(&conn);

		/* There should NOT be a host notification */
		ut_rx_q_is_empty();
	}

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should NOT have a LL Control PDU */
	lt_rx_q_is_empty(&conn);

	/* Done */
	event_done(&conn);

	/* There should be one host notification */
	ut_rx_node(NODE_PHY_UPDATE, &ntf, &pu);
	ut_rx_q_is_empty();

	/* Release Ntf */
	ull_cp_release_ntf(ntf);

	zassert_equal(ctx_buffers_free(), PROC_CTX_BUF_NUM, "Free CTX buffers %d", ctx_buffers_free());
}

void test_phy_update_sla_loc_collision(void)
{
	uint8_t err;
	struct node_tx *tx;
	struct node_rx_pdu *ntf;
	uint16_t instant;

	struct pdu_data_llctrl_reject_ext_ind reject_ext_ind = {
		.reject_opcode = PDU_DATA_LLCTRL_TYPE_PHY_REQ,
		.error_code = BT_HCI_ERR_LL_PROC_COLLISION
	};

	struct node_rx_pu pu = {0};
	struct pdu_data_llctrl_phy_upd_ind phy_update_ind = {0};

	/* Role */
	test_set_role(&conn, BT_HCI_ROLE_SLAVE);

	/* Connect */
	ull_cp_state_set(&conn, ULL_CP_CONNECTED);

	/*** ***/

	/* Initiate an PHY Update Procedure */
	err = ull_cp_phy_update(&conn);
	zassert_equal(err, BT_HCI_ERR_SUCCESS, NULL);

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should have one LL Control PDU */
	lt_rx(LL_PHY_REQ, &conn, &tx, NULL);
	lt_rx_q_is_empty(&conn);

	/* Rx */
	lt_tx(LL_PHY_REQ, &conn, NULL);

	/* Done */
	event_done(&conn);

	/* Release Tx */
	ull_cp_release_tx(tx);

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should have one LL Control PDU */
	lt_rx(LL_PHY_RSP, &conn, &tx, NULL);
	lt_rx_q_is_empty(&conn);

	/* Rx */
	lt_tx(LL_REJECT_EXT_IND, &conn, &reject_ext_ind);

	/* Done */
	event_done(&conn);

	/* There should be one host notification */
	pu.status = BT_HCI_ERR_LL_PROC_COLLISION;
	ut_rx_node(NODE_PHY_UPDATE, &ntf, &pu);
	ut_rx_q_is_empty();

	/* Release Ntf */
	ull_cp_release_ntf(ntf);

	/* Prepare */
	event_prepare(&conn);

	/* Rx */
	phy_update_ind.instant = instant = event_counter(&conn) + 6;
	lt_tx(LL_PHY_UPDATE_IND, &conn, &phy_update_ind);

	/* Done */
	event_done(&conn);

	/* Release Tx */
	ull_cp_release_tx(tx);

	/* */
	while (!is_instant_reached(&conn, instant)) {
		/* Prepare */
		event_prepare(&conn);

		/* Tx Queue should NOT have a LL Control PDU */
		lt_rx_q_is_empty(&conn);

		/* Done */
		event_done(&conn);

		/* There should NOT be a host notification */
		ut_rx_q_is_empty();
	}

	/* Prepare */
	event_prepare(&conn);

	/* Tx Queue should NOT have a LL Control PDU */
	lt_rx_q_is_empty(&conn);

	/* Done */
	event_done(&conn);

	/* There should be one host notification */
	pu.status = BT_HCI_ERR_SUCCESS;
	ut_rx_node(NODE_PHY_UPDATE, &ntf, &pu);
	ut_rx_q_is_empty();

	/* Release Ntf */
	ull_cp_release_ntf(ntf);

	zassert_equal(ctx_buffers_free(), PROC_CTX_BUF_NUM, "Free CTX buffers %d", ctx_buffers_free());
}


void test_main(void)
{

	ztest_test_suite(phy,
			 ztest_unit_test_setup_teardown(test_phy_update_mas_loc, setup, unit_test_noop),
			 ztest_unit_test_setup_teardown(test_phy_update_mas_loc_unsupp_feat, setup, unit_test_noop),
			 ztest_unit_test_setup_teardown(test_phy_update_mas_rem, setup, unit_test_noop),
			 ztest_unit_test_setup_teardown(test_phy_update_sla_loc, setup, unit_test_noop),
			 ztest_unit_test_setup_teardown(test_phy_update_sla_rem, setup, unit_test_noop),
			 ztest_unit_test_setup_teardown(test_phy_update_mas_loc_collision, setup, unit_test_noop),
			 ztest_unit_test_setup_teardown(test_phy_update_mas_rem_collision, setup, unit_test_noop),
			 ztest_unit_test_setup_teardown(test_phy_update_sla_loc_collision, setup, unit_test_noop)
			);

	ztest_run_test_suite(phy);
}
