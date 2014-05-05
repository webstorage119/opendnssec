/*
 * Copyright (c) 2014 Jerry Lundström <lundstrom.jerry@gmail.com>
 * Copyright (c) 2014 .SE (The Internet Infrastructure Foundation).
 * Copyright (c) 2014 OpenDNSSEC AB (svb)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "policy.h"

#include "db_error.h"
#include "shared/duration.h"
#include "shared/log.h"

#include <string.h>

int policy_create_from_xmlNode(policy_t* policy, xmlNodePtr policy_node) {
    xmlNodePtr node;
    xmlNodePtr node2;
    xmlNodePtr node3;
    xmlNodePtr node4;
    xmlChar* xml_text;
    duration_type* duration;

    if (!policy) {
        return DB_ERROR_UNKNOWN;
    }
    if (!policy_node) {
        return DB_ERROR_UNKNOWN;
    }

    if (!(xml_text = xmlGetProp(policy_node, (xmlChar*)"name"))) {
        return DB_ERROR_UNKNOWN;
    }
    ods_log_deeebug("policy_create_from_xmlNode: policy %s", (char*)xml_text);
    if (policy_set_name(policy, (char*)xml_text)) {
        xmlFree(xml_text);
        return DB_ERROR_UNKNOWN;
    }
    xmlFree(xml_text);

    for (node = policy_node->children; node; node = node->next) {
        if (node->type != XML_ELEMENT_NODE) {
            continue;
        }

        if (!strcmp((char*)node->name, "Description")) {
            if (!(xml_text = xmlNodeGetContent(node))) {
                return DB_ERROR_UNKNOWN;
            }
            ods_log_deeebug("policy_create_from_xmlNode: description %s", (char*)xml_text);
            if (policy_set_description(policy, (char*)xml_text)) {
                xmlFree(xml_text);
                return DB_ERROR_UNKNOWN;
            }
            xmlFree(xml_text);
        }
        else if (!strcmp((char*)node->name, "Signatures")) {
            for (node2 = node->children; node2; node2 = node2->next) {
                if (node2->type != XML_ELEMENT_NODE) {
                    continue;
                }

                if (!strcmp((char*)node2->name, "Resign")) {
                    if (!(xml_text = xmlNodeGetContent(node2))) {
                        return DB_ERROR_UNKNOWN;
                    }
                    ods_log_deeebug("policy_create_from_xmlNode: signature resign %s", (char*)xml_text);
                    if (!(duration = duration_create_from_string((char*)xml_text))) {
                        xmlFree(xml_text);
                        return DB_ERROR_UNKNOWN;
                    }
                    xmlFree(xml_text);
                    if (policy_set_signatures_resign(policy, duration2time(duration))) {
                        duration_cleanup(duration);
                        return DB_ERROR_UNKNOWN;
                    }
                    duration_cleanup(duration);
                }
                else if (!strcmp((char*)node2->name, "Refresh")) {
                    if (!(xml_text = xmlNodeGetContent(node2))) {
                        return DB_ERROR_UNKNOWN;
                    }
                    ods_log_deeebug("policy_create_from_xmlNode: signature refresh %s", (char*)xml_text);
                    if (!(duration = duration_create_from_string((char*)xml_text))) {
                        xmlFree(xml_text);
                        return DB_ERROR_UNKNOWN;
                    }
                    xmlFree(xml_text);
                    if (policy_set_signatures_refresh(policy, duration2time(duration))) {
                        duration_cleanup(duration);
                        return DB_ERROR_UNKNOWN;
                    }
                    duration_cleanup(duration);
                }
                else if (!strcmp((char*)node2->name, "Validity")) {
                    for (node3 = node2->children; node3; node3 = node3->next) {
                        if (node3->type != XML_ELEMENT_NODE) {
                            continue;
                        }

                        if (!strcmp((char*)node3->name, "Default")) {
                            if (!(xml_text = xmlNodeGetContent(node3))) {
                                return DB_ERROR_UNKNOWN;
                            }
                            ods_log_deeebug("policy_create_from_xmlNode: signature validity default %s", (char*)xml_text);
                            if (!(duration = duration_create_from_string((char*)xml_text))) {
                                xmlFree(xml_text);
                                return DB_ERROR_UNKNOWN;
                            }
                            xmlFree(xml_text);
                            if (policy_set_signatures_validity_default(policy, duration2time(duration))) {
                                duration_cleanup(duration);
                                return DB_ERROR_UNKNOWN;
                            }
                            duration_cleanup(duration);
                        }
                        else if (!strcmp((char*)node3->name, "Denial")) {
                            if (!(xml_text = xmlNodeGetContent(node3))) {
                                return DB_ERROR_UNKNOWN;
                            }
                            ods_log_deeebug("policy_create_from_xmlNode: signature validity denial %s", (char*)xml_text);
                            if (!(duration = duration_create_from_string((char*)xml_text))) {
                                xmlFree(xml_text);
                                return DB_ERROR_UNKNOWN;
                            }
                            xmlFree(xml_text);
                            if (policy_set_signatures_validity_denial(policy, duration2time(duration))) {
                                duration_cleanup(duration);
                                return DB_ERROR_UNKNOWN;
                            }
                            duration_cleanup(duration);
                        }
                        else {
                            ods_log_deeebug("policy_create_from_xmlNode: unknown %s", (char*)node3->name);
                            return DB_ERROR_UNKNOWN;
                        }
                    }
                }
                else if (!strcmp((char*)node2->name, "Jitter")) {
                    if (!(xml_text = xmlNodeGetContent(node2))) {
                        return DB_ERROR_UNKNOWN;
                    }
                    ods_log_deeebug("policy_create_from_xmlNode: signature jitter %s", (char*)xml_text);
                    if (!(duration = duration_create_from_string((char*)xml_text))) {
                        xmlFree(xml_text);
                        return DB_ERROR_UNKNOWN;
                    }
                    xmlFree(xml_text);
                    if (policy_set_signatures_jitter(policy, duration2time(duration))) {
                        duration_cleanup(duration);
                        return DB_ERROR_UNKNOWN;
                    }
                    duration_cleanup(duration);
                }
                else if (!strcmp((char*)node2->name, "InceptionOffset")) {
                    if (!(xml_text = xmlNodeGetContent(node2))) {
                        return DB_ERROR_UNKNOWN;
                    }
                    ods_log_deeebug("policy_create_from_xmlNode: signature inception offset %s", (char*)xml_text);
                    if (!(duration = duration_create_from_string((char*)xml_text))) {
                        xmlFree(xml_text);
                        return DB_ERROR_UNKNOWN;
                    }
                    xmlFree(xml_text);
                    if (policy_set_signatures_inception_offset(policy, duration2time(duration))) {
                        duration_cleanup(duration);
                        return DB_ERROR_UNKNOWN;
                    }
                    duration_cleanup(duration);
                }
                else if (!strcmp((char*)node2->name, "MaxZoneTTL")) {
                    if (!(xml_text = xmlNodeGetContent(node2))) {
                        return DB_ERROR_UNKNOWN;
                    }
                    ods_log_deeebug("policy_create_from_xmlNode: signature max zone ttl %s", (char*)xml_text);
                    if (!(duration = duration_create_from_string((char*)xml_text))) {
                        xmlFree(xml_text);
                        return DB_ERROR_UNKNOWN;
                    }
                    xmlFree(xml_text);
                    if (policy_set_signatures_max_zone_ttl(policy, duration2time(duration))) {
                        duration_cleanup(duration);
                        return DB_ERROR_UNKNOWN;
                    }
                    duration_cleanup(duration);
                }
                else {
                    ods_log_deeebug("policy_create_from_xmlNode: unknown %s", (char*)node2->name);
                    return DB_ERROR_UNKNOWN;
                }
            }
        }
        else if (!strcmp((char*)node->name, "Denial")) {
            for (node2 = node->children; node2; node2 = node2->next) {
                if (node2->type != XML_ELEMENT_NODE) {
                    continue;
                }

                if (!strcmp((char*)node2->name, "NSEC")) {
                    ods_log_deeebug("policy_create_from_xmlNode: denial nsec");
                    if (policy_set_denial_type(policy, POLICY_DENIAL_TYPE_NSEC)) {
                        return DB_ERROR_UNKNOWN;
                    }
                }
                else if (!strcmp((char*)node2->name, "NSEC3")) {
                    ods_log_deeebug("policy_create_from_xmlNode: denial nsec3");
                    if (policy_set_denial_type(policy, POLICY_DENIAL_TYPE_NSEC3)) {
                        return DB_ERROR_UNKNOWN;
                    }

                    for (node3 = node2->children; node3; node3 = node3->next) {
                        if (node3->type != XML_ELEMENT_NODE) {
                            continue;
                        }

                        if (!strcmp((char*)node3->name, "TTL")) {
                            if (!(xml_text = xmlNodeGetContent(node3))) {
                                return DB_ERROR_UNKNOWN;
                            }
                            ods_log_deeebug("policy_create_from_xmlNode: denial ttl %s", (char*)xml_text);
                            if (!(duration = duration_create_from_string((char*)xml_text))) {
                                xmlFree(xml_text);
                                return DB_ERROR_UNKNOWN;
                            }
                            xmlFree(xml_text);
                            if (policy_set_denial_ttl(policy, duration2time(duration))) {
                                duration_cleanup(duration);
                                return DB_ERROR_UNKNOWN;
                            }
                            duration_cleanup(duration);
                        }
                        else if (!strcmp((char*)node3->name, "OptOut")) {
                            ods_log_deeebug("policy_create_from_xmlNode: denial optout");
                            if (policy_set_denial_optout(policy, 1)) {
                                return DB_ERROR_UNKNOWN;
                            }
                        }
                        else if (!strcmp((char*)node3->name, "Resalt")) {
                            if (!(xml_text = xmlNodeGetContent(node3))) {
                                return DB_ERROR_UNKNOWN;
                            }
                            ods_log_deeebug("policy_create_from_xmlNode: denial resalt %s", (char*)xml_text);
                            if (!(duration = duration_create_from_string((char*)xml_text))) {
                                xmlFree(xml_text);
                                return DB_ERROR_UNKNOWN;
                            }
                            xmlFree(xml_text);
                            if (policy_set_denial_resalt(policy, duration2time(duration))) {
                                duration_cleanup(duration);
                                return DB_ERROR_UNKNOWN;
                            }
                            duration_cleanup(duration);
                        }
                        else if (!strcmp((char*)node3->name, "Hash")) {
                            for (node4 = node3->children; node4; node4 = node4->next) {
                                if (node4->type != XML_ELEMENT_NODE) {
                                    continue;
                                }

                                if (!strcmp((char*)node4->name, "Algorithm")) {
                                    if (!(xml_text = xmlNodeGetContent(node4))) {
                                        return DB_ERROR_UNKNOWN;
                                    }
                                    ods_log_deeebug("policy_create_from_xmlNode: denial algorithm %s", (char*)xml_text);
                                    if (policy_set_denial_algorithm(policy, atoi((char*)xml_text))) {
                                        xmlFree(xml_text);
                                        return DB_ERROR_UNKNOWN;
                                    }
                                    xmlFree(xml_text);
                                }
                                else if (!strcmp((char*)node4->name, "Iterations")) {
                                    if (!(xml_text = xmlNodeGetContent(node4))) {
                                        return DB_ERROR_UNKNOWN;
                                    }
                                    ods_log_deeebug("policy_create_from_xmlNode: denial iterations %s", (char*)xml_text);
                                    if (policy_set_denial_iterations(policy, atoi((char*)xml_text))) {
                                        xmlFree(xml_text);
                                        return DB_ERROR_UNKNOWN;
                                    }
                                    xmlFree(xml_text);
                                }
                                else if (!strcmp((char*)node4->name, "Salt")) {
                                    if (!(xml_text = xmlGetProp(node4, (xmlChar*)"length"))) {
                                        return DB_ERROR_UNKNOWN;
                                    }
                                    ods_log_deeebug("policy_create_from_xmlNode: denial salt length %s", (char*)xml_text);
                                    if (policy_set_denial_salt_length(policy, atoi((char*)xml_text))) {
                                        xmlFree(xml_text);
                                        return DB_ERROR_UNKNOWN;
                                    }
                                    xmlFree(xml_text);
                                }
                                else {
                                    ods_log_deeebug("policy_create_from_xmlNode: unknown %s", (char*)node4->name);
                                    return DB_ERROR_UNKNOWN;
                                }
                            }
                        }
                        else {
                            ods_log_deeebug("policy_create_from_xmlNode: unknown %s", (char*)node3->name);
                            return DB_ERROR_UNKNOWN;
                        }
                    }
                }
                else {
                    ods_log_deeebug("policy_create_from_xmlNode: unknown %s", (char*)node2->name);
                    return DB_ERROR_UNKNOWN;
                }
            }
        }
        else if (!strcmp((char*)node->name, "Keys")) {
            for (node2 = node->children; node2; node2 = node2->next) {
                if (node2->type != XML_ELEMENT_NODE) {
                    continue;
                }

                if (!strcmp((char*)node2->name, "TTL")) {
                    if (!(xml_text = xmlNodeGetContent(node2))) {
                        return DB_ERROR_UNKNOWN;
                    }
                    ods_log_deeebug("policy_create_from_xmlNode: keys ttl %s", (char*)xml_text);
                    if (!(duration = duration_create_from_string((char*)xml_text))) {
                        xmlFree(xml_text);
                        return DB_ERROR_UNKNOWN;
                    }
                    xmlFree(xml_text);
                    if (policy_set_keys_ttl(policy, duration2time(duration))) {
                        duration_cleanup(duration);
                        return DB_ERROR_UNKNOWN;
                    }
                    duration_cleanup(duration);
                }
                else if (!strcmp((char*)node2->name, "RetireSafety")) {
                    if (!(xml_text = xmlNodeGetContent(node2))) {
                        return DB_ERROR_UNKNOWN;
                    }
                    ods_log_deeebug("policy_create_from_xmlNode: keys retire safety %s", (char*)xml_text);
                    if (!(duration = duration_create_from_string((char*)xml_text))) {
                        xmlFree(xml_text);
                        return DB_ERROR_UNKNOWN;
                    }
                    xmlFree(xml_text);
                    if (policy_set_keys_retire_safety(policy, duration2time(duration))) {
                        duration_cleanup(duration);
                        return DB_ERROR_UNKNOWN;
                    }
                    duration_cleanup(duration);
                }
                else if (!strcmp((char*)node2->name, "PublishSafety")) {
                    if (!(xml_text = xmlNodeGetContent(node2))) {
                        return DB_ERROR_UNKNOWN;
                    }
                    ods_log_deeebug("policy_create_from_xmlNode: keys publish safety %s", (char*)xml_text);
                    if (!(duration = duration_create_from_string((char*)xml_text))) {
                        xmlFree(xml_text);
                        return DB_ERROR_UNKNOWN;
                    }
                    xmlFree(xml_text);
                    if (policy_set_keys_publish_safety(policy, duration2time(duration))) {
                        duration_cleanup(duration);
                        return DB_ERROR_UNKNOWN;
                    }
                    duration_cleanup(duration);
                }
                else if (!strcmp((char*)node2->name, "ShareKeys")) {
                    ods_log_deeebug("policy_create_from_xmlNode: keys shared keys");
                    if (policy_set_keys_shared(policy, 1)) {
                        return DB_ERROR_UNKNOWN;
                    }
                }
                else if (!strcmp((char*)node2->name, "Purge")) {
                    if (!(xml_text = xmlNodeGetContent(node2))) {
                        return DB_ERROR_UNKNOWN;
                    }
                    ods_log_deeebug("policy_create_from_xmlNode: keys purge %s", (char*)xml_text);
                    if (!(duration = duration_create_from_string((char*)xml_text))) {
                        xmlFree(xml_text);
                        return DB_ERROR_UNKNOWN;
                    }
                    xmlFree(xml_text);
                    if (policy_set_keys_purge_after(policy, duration2time(duration))) {
                        duration_cleanup(duration);
                        return DB_ERROR_UNKNOWN;
                    }
                    duration_cleanup(duration);
                }
                else if (!strcmp((char*)node2->name, "KSK")) {
                    continue;
                }
                else if (!strcmp((char*)node2->name, "ZSK")) {
                    continue;
                }
                else if (!strcmp((char*)node2->name, "CSK")) {
                    continue;
                }
                else {
                    ods_log_deeebug("policy_create_from_xmlNode: unknown %s", (char*)node2->name);
                    return DB_ERROR_UNKNOWN;
                }
            }
        }
        else if (!strcmp((char*)node->name, "Zone")) {
            for (node2 = node->children; node2; node2 = node2->next) {
                if (node2->type != XML_ELEMENT_NODE) {
                    continue;
                }

                if (!strcmp((char*)node2->name, "PropagationDelay")) {
                    if (!(xml_text = xmlNodeGetContent(node2))) {
                        return DB_ERROR_UNKNOWN;
                    }
                    ods_log_deeebug("policy_create_from_xmlNode: zone propagation delay %s", (char*)xml_text);
                    if (!(duration = duration_create_from_string((char*)xml_text))) {
                        xmlFree(xml_text);
                        return DB_ERROR_UNKNOWN;
                    }
                    xmlFree(xml_text);
                    if (policy_set_zone_propagation_delay(policy, duration2time(duration))) {
                        duration_cleanup(duration);
                        return DB_ERROR_UNKNOWN;
                    }
                    duration_cleanup(duration);
                }
                else if (!strcmp((char*)node2->name, "SOA")) {
                    for (node3 = node2->children; node3; node3 = node3->next) {
                        if (node3->type != XML_ELEMENT_NODE) {
                            continue;
                        }

                        if (!strcmp((char*)node3->name, "TTL")) {
                            if (!(xml_text = xmlNodeGetContent(node3))) {
                                return DB_ERROR_UNKNOWN;
                            }
                            ods_log_deeebug("policy_create_from_xmlNode: zone soa ttl %s", (char*)xml_text);
                            if (!(duration = duration_create_from_string((char*)xml_text))) {
                                xmlFree(xml_text);
                                return DB_ERROR_UNKNOWN;
                            }
                            xmlFree(xml_text);
                            if (policy_set_zone_soa_ttl(policy, duration2time(duration))) {
                                duration_cleanup(duration);
                                return DB_ERROR_UNKNOWN;
                            }
                            duration_cleanup(duration);
                        }
                        else if (!strcmp((char*)node3->name, "Minimum")) {
                            if (!(xml_text = xmlNodeGetContent(node3))) {
                                return DB_ERROR_UNKNOWN;
                            }
                            ods_log_deeebug("policy_create_from_xmlNode: zone soa minimum %s", (char*)xml_text);
                            if (!(duration = duration_create_from_string((char*)xml_text))) {
                                xmlFree(xml_text);
                                return DB_ERROR_UNKNOWN;
                            }
                            xmlFree(xml_text);
                            if (policy_set_zone_soa_minimum(policy, duration2time(duration))) {
                                duration_cleanup(duration);
                                return DB_ERROR_UNKNOWN;
                            }
                            duration_cleanup(duration);
                        }
                        else if (!strcmp((char*)node3->name, "Serial")) {
                            if (!(xml_text = xmlNodeGetContent(node3))) {
                                return DB_ERROR_UNKNOWN;
                            }
                            ods_log_deeebug("policy_create_from_xmlNode: zone soa serial %s", (char*)xml_text);
                            if (policy_set_zone_soa_serial_text(policy, (char*)xml_text)) {
                                xmlFree(xml_text);
                                return DB_ERROR_UNKNOWN;
                            }
                            xmlFree(xml_text);
                        }
                        else {
                            ods_log_deeebug("policy_create_from_xmlNode: unknown %s", (char*)node3->name);
                            return DB_ERROR_UNKNOWN;
                        }
                    }
                }
                else {
                    ods_log_deeebug("policy_create_from_xmlNode: unknown %s", (char*)node2->name);
                    return DB_ERROR_UNKNOWN;
                }
            }
        }
        else if (!strcmp((char*)node->name, "Parent")) {
            for (node2 = node->children; node2; node2 = node2->next) {
                if (node2->type != XML_ELEMENT_NODE) {
                    continue;
                }

                if (!strcmp((char*)node2->name, "PropagationDelay")) {
                    if (!(xml_text = xmlNodeGetContent(node2))) {
                        return DB_ERROR_UNKNOWN;
                    }
                    ods_log_deeebug("policy_create_from_xmlNode: parent propagation delay %s", (char*)xml_text);
                    if (!(duration = duration_create_from_string((char*)xml_text))) {
                        xmlFree(xml_text);
                        return DB_ERROR_UNKNOWN;
                    }
                    xmlFree(xml_text);
                    if (policy_set_parent_propagation_delay(policy, duration2time(duration))) {
                        duration_cleanup(duration);
                        return DB_ERROR_UNKNOWN;
                    }
                    duration_cleanup(duration);
                }
                else if (!strcmp((char*)node2->name, "SOA")) {
                    for (node3 = node2->children; node3; node3 = node3->next) {
                        if (node3->type != XML_ELEMENT_NODE) {
                            continue;
                        }

                        if (!strcmp((char*)node3->name, "TTL")) {
                            if (!(xml_text = xmlNodeGetContent(node3))) {
                                return DB_ERROR_UNKNOWN;
                            }
                            ods_log_deeebug("policy_create_from_xmlNode: parent soa ttl %s", (char*)xml_text);
                            if (!(duration = duration_create_from_string((char*)xml_text))) {
                                xmlFree(xml_text);
                                return DB_ERROR_UNKNOWN;
                            }
                            xmlFree(xml_text);
                            if (policy_set_parent_soa_ttl(policy, duration2time(duration))) {
                                duration_cleanup(duration);
                                return DB_ERROR_UNKNOWN;
                            }
                            duration_cleanup(duration);
                        }
                        else if (!strcmp((char*)node3->name, "Minimum")) {
                            if (!(xml_text = xmlNodeGetContent(node3))) {
                                return DB_ERROR_UNKNOWN;
                            }
                            ods_log_deeebug("policy_create_from_xmlNode: parent soa minimum %s", (char*)xml_text);
                            if (!(duration = duration_create_from_string((char*)xml_text))) {
                                xmlFree(xml_text);
                                return DB_ERROR_UNKNOWN;
                            }
                            xmlFree(xml_text);
                            if (policy_set_parent_soa_minimum(policy, duration2time(duration))) {
                                duration_cleanup(duration);
                                return DB_ERROR_UNKNOWN;
                            }
                            duration_cleanup(duration);
                        }
                        else {
                            ods_log_deeebug("policy_create_from_xmlNode: unknown %s", (char*)node3->name);
                            return DB_ERROR_UNKNOWN;
                        }
                    }
                }
                else if (!strcmp((char*)node2->name, "DS")) {
                    for (node3 = node2->children; node3; node3 = node3->next) {
                        if (node3->type != XML_ELEMENT_NODE) {
                            continue;
                        }

                        if (!strcmp((char*)node3->name, "TTL")) {
                            if (!(xml_text = xmlNodeGetContent(node3))) {
                                return DB_ERROR_UNKNOWN;
                            }
                            ods_log_deeebug("policy_create_from_xmlNode: parent ds ttl %s", (char*)xml_text);
                            if (!(duration = duration_create_from_string((char*)xml_text))) {
                                xmlFree(xml_text);
                                return DB_ERROR_UNKNOWN;
                            }
                            xmlFree(xml_text);
                            if (policy_set_parent_ds_ttl(policy, duration2time(duration))) {
                                duration_cleanup(duration);
                                return DB_ERROR_UNKNOWN;
                            }
                            duration_cleanup(duration);
                        }
                        else {
                            ods_log_deeebug("policy_create_from_xmlNode: unknown %s", (char*)node3->name);
                            return DB_ERROR_UNKNOWN;
                        }
                    }
                }
                else {
                    ods_log_deeebug("policy_create_from_xmlNode: unknown %s", (char*)node2->name);
                    return DB_ERROR_UNKNOWN;
                }
            }
        }
        else {
            ods_log_deeebug("policy_create_from_xmlNode: unknown %s", (char*)node->name);
            return DB_ERROR_UNKNOWN;
        }
    }

    return DB_OK;
}

int policy_update_from_xmlNode(policy_t* policy, xmlNodePtr policy_node, int* updated) {
    if (!policy) {
        return DB_ERROR_UNKNOWN;
    }
    if (!policy_node) {
        return DB_ERROR_UNKNOWN;
    }
    if (!updated) {
        return DB_ERROR_UNKNOWN;
    }

    *updated = 0;

    return DB_OK;
}
