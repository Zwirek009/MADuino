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
	REQUEST,
	PROPOSE,
	REFUSE,
	AGREE
};

enum language
{
	MADUINO
};

enum ontology
{
	NO_ONTOLOGY
};

enum protocol
{
	NO_PROTOCOL,
	REQUEST_INTERACTION_PROTOCOL
};

#endif