/// Project: MADuino
/// Author: Maciej Wiraszka
///
/// MADuino - Arduino library for creating multi-agent systems based on Arduino boards.
///
/// File: Enums.h - heather file for holding enums associated with FIPA ACL standards
///

#ifndef ENUMS_H
#define ENUMS_H

enum performative
{
	ACCEPT_PROPOSAL,		// 0
	AGREE,					// 1
	CANCEL,					// 2
	CFP,					// 3
	CONFIRM,				// 4
	DISCONFIRM,				// 5
	FAILURE,				// 6
	INFORM,					// 7
	INFORM_IF,				// 8
	INFORM_REF,				// 9
	NOT_UNDERSTOOD,			// 10
	PROPAGATE,				// 11
	PROPOSE,				// 12
	PROXY,					// 13
	QUERY_IF,				// 14
	QUERY_REF,				// 15
	REFUSE,					// 16
	REJECT_PROPOSAL,		// 17
	REQUEST,				// 18
	REQUEST_WHEN,			// 19
	REQUEST_WHENEVER,		// 20
	SUBSCRIBE				// 21
};

enum language
{
	USER_DEFINED			// 0
};

enum ontology
{
	NO_ONTOLOGY				// 0
};

enum encoding
{
	NOT_DEFINED				// 0
};

enum protocol
{
	NO_PROTOCOL,					// 0
	REQUEST_INTERACTION_PROTOCOL,	// 1
	PROPOSE_INTERACTION_PROTOCOL,	// 2
	CONTRACT_NET_PROTOCOL			// 3
};

#endif