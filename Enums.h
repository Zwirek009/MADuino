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
	ACCEPT_PROPOSAL,
	AGREE,
	CANCEL,
	CFP,
	CONFIRM,
	DISCONFIRM,
	FAILURE,
	INFORM,
	INFORM_IF,
	INFORM_REF,
	NOT_UNDERSTOOD,
	PROPAGATE,
	PROPOSE,
	PROXY,
	QUERY_IF,
	QUERY_REF,
	REFUSE,
	REJECT_PROPOSAL,
	REQUEST,
	REQUEST_WHEN,
	REQUEST_WHENEVER,
	SUBSCRIBE
};

enum language
{
	USER_DEFINED
};

enum ontology
{
	NO_ONTOLOGY
};

enum protocol
{
	NO_PROTOCOL,
	REQUEST_INTERACTION_PROTOCOL,
	PROPOSE_INTERACTION_PROTOCOL,
	CONTRACT_NET_PROTOCOL
};

#endif