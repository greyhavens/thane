/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is [Open Source Virtual Machine].
 *
 * The Initial Developer of the Original Code is
 * Adobe System Incorporated.
 * Portions created by the Initial Developer are Copyright (C) 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Adobe AS3 Team
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

/* machine generated file -- do not edit */

#define AVMTHUNK_VERSION 5

extern const uint32_t thanelib_abc_class_count;
extern const uint32_t thanelib_abc_script_count;
extern const uint32_t thanelib_abc_method_count;
extern const uint32_t thanelib_abc_length;
extern const uint8_t thanelib_abc_data[];
AVMTHUNK_DECLARE_NATIVE_INITIALIZER(thanelib)

/* classes */
const uint32_t abcclass_mx_core_IRawChildrenContainer = 0;
const uint32_t abcclass_mx_core_IChildList = 1;
const uint32_t abcclass_mx_resources_Locale = 2;
const uint32_t abcclass_mx_resources_ResourceManager = 3;
const uint32_t abcclass_mx_resources_IResourceBundle = 4;
const uint32_t abcclass_mx_resources_IResourceManager = 5;
const uint32_t abcclass_avmplus_Domain = 6;
const uint32_t abcclass_org_httpclient_http_multipart_Multipart = 7;
const uint32_t abcclass_org_httpclient_http_multipart_Part = 8;
const uint32_t abcclass_org_httpclient_events_HttpListener = 9;
const uint32_t abcclass_org_httpclient_io_HttpResponseBuffer = 10;
const uint32_t abcclass_org_httpclient_io_HttpRequestBuffer = 11;
const uint32_t abcclass_org_httpclient_io_HttpBuffer = 12;
const uint32_t abcclass_org_httpclient_HttpResponse = 13;
const uint32_t abcclass_org_httpclient_HttpTimer = 14;
const uint32_t abcclass_org_httpclient_Log = 15;
const uint32_t abcclass_org_httpclient_HttpHeader = 16;
const uint32_t abcclass_org_httpclient_HttpRequest = 17;
const uint32_t abcclass_org_httpclient_HttpSocket = 18;
const uint32_t abcclass_de_polygonal_ds_Iterator = 19;
const uint32_t abcclass_de_polygonal_ds_Collection = 20;
const uint32_t abcclass_flash_utils_Endian = 21;
const uint32_t abcclass_flash_utils_IDataInput = 22;
const uint32_t abcclass_flash_utils_IExternalizable = 23;
const uint32_t abcclass_flash_utils_IDataOutput = 24;
const uint32_t abcclass_flash_utils_Dictionary = 25;
const uint32_t abcclass_flash_events_Event = 26;
const uint32_t abcclass_flash_events_IEventDispatcher = 27;
const uint32_t abcclass_flash_display_StageQuality = 28;
const uint32_t abcclass_flash_net_URLLoaderDataFormat = 29;
const uint32_t abcclass_flash_net_ObjectEncoding = 30;
const uint32_t abcclass_flash_net_URLRequest = 31;
const uint32_t abcclass_flash_geom_Matrix = 32;
const uint32_t abcclass_flash_geom_Rectangle = 33;
const uint32_t abcclass_flash_geom_Point = 34;
const uint32_t abcclass_flash_errors_MemoryError = 35;
const uint32_t abcclass_flash_errors_IOError = 36;
const uint32_t abcclass_flash_errors_EOFError = 37;
const uint32_t abcclass_flash_errors_IllegalOperationError = 38;
const uint32_t abcclass_com_adobe_utils_StringUtil = 39;
const uint32_t abcclass_com_adobe_utils_XMLUtil = 40;
const uint32_t abcclass_com_adobe_utils_DictionaryUtil = 41;
const uint32_t abcclass_com_adobe_utils_IntUtil = 42;
const uint32_t abcclass_com_adobe_utils_ArrayUtil = 43;
const uint32_t abcclass_com_adobe_utils_NumberFormatter = 44;
const uint32_t abcclass_com_adobe_net_URI = 45;
const uint32_t abcclass_com_adobe_net_MimeTypeMap = 46;
const uint32_t abcclass_com_adobe_net_IURIResolver = 47;
const uint32_t abcclass_com_adobe_errors_IllegalStateError = 48;
const uint32_t abcclass_com_adobe_serialization_json_JSONParseError = 49;
const uint32_t abcclass_com_adobe_serialization_json_JSONDecoder = 50;
const uint32_t abcclass_com_adobe_serialization_json_JSONTokenizer = 51;
const uint32_t abcclass_com_adobe_serialization_json_JSON = 52;
const uint32_t abcclass_com_adobe_serialization_json_JSONEncoder = 53;
const uint32_t abcclass_com_adobe_serialization_json_JSONToken = 54;
const uint32_t abcclass_com_adobe_serialization_json_JSONTokenType = 55;
const uint32_t abcclass_Thanette = 56;
const uint32_t abcclass_Thane = 57;
const uint32_t abcclass_private_SpawnedDomain = 58;
const uint32_t abcclass_AMF3 = 59;
const uint32_t abcclass_AMF3Encoder = 60;
const uint32_t abcclass_private_EncoderContext = 61;
const uint32_t abcclass_private_References = 62;
const uint32_t abcclass_AMF3Decoder = 63;
const uint32_t abcclass_private_DecoderContext = 64;
const uint32_t abcclass_private_Traits = 65;
const uint32_t abcclass_avmplus_System = 66;
const uint32_t abcclass_flash_system_Capabilities = 67;
const uint32_t abcclass_flash_system_System = 68;
const uint32_t abcclass_org_httpclient_http_Delete = 69;
const uint32_t abcclass_org_httpclient_http_Get = 70;
const uint32_t abcclass_org_httpclient_http_Head = 71;
const uint32_t abcclass_org_httpclient_http_Post = 72;
const uint32_t abcclass_org_httpclient_http_Put = 73;
const uint32_t abcclass_de_polygonal_ds_Heap = 74;
const uint32_t abcclass_private_HeapIterator = 75;
const uint32_t abcclass_flash_utils_ByteArray = 76;
const uint32_t abcclass_org_httpclient_events_HttpStatusEvent = 77;
const uint32_t abcclass_org_httpclient_events_HttpDataEvent = 78;
const uint32_t abcclass_flash_events_TextEvent = 79;
const uint32_t abcclass_flash_events_KeyboardEvent = 80;
const uint32_t abcclass_flash_events_TimerEvent = 81;
const uint32_t abcclass_flash_events_ProgressEvent = 82;
const uint32_t abcclass_flash_events_MouseEvent = 83;
const uint32_t abcclass_flash_events_NetStatusEvent = 84;
const uint32_t abcclass_TraceEvent = 85;
const uint32_t abcclass_flash_events_EventDispatcher = 86;
const uint32_t abcclass_com_adobe_net_URIEncodingBitmap = 87;
const uint32_t abcclass_flash_events_ErrorEvent = 88;
const uint32_t abcclass_org_httpclient_HttpClient = 89;
const uint32_t abcclass_flash_utils_Timer = 90;
const uint32_t abcclass_private_Buddy = 91;
const uint32_t abcclass_flash_display_LoaderInfo = 92;
const uint32_t abcclass_flash_display_DisplayObject = 93;
const uint32_t abcclass_flash_net_Socket = 94;
const uint32_t abcclass_flash_net_URLLoader = 95;
const uint32_t abcclass_org_httpclient_events_HttpErrorEvent = 96;
const uint32_t abcclass_flash_events_SecurityErrorEvent = 97;
const uint32_t abcclass_flash_events_IOErrorEvent = 98;
const uint32_t abcclass_flash_events_HTTPStatusEvent = 99;
const uint32_t abcclass_flash_display_DisplayObjectContainer = 100;
const uint32_t abcclass_com_adobe_net_proxies_RFC2817Socket = 101;

/* methods */
const uint32_t avmplus_Domain_currentDomain_get = 39;
const uint32_t avmplus_Domain_MIN_DOMAIN_MEMORY_LENGTH_get = 40;
const uint32_t avmplus_Domain_private_initNewDomain = 42;
const uint32_t avmplus_Domain_private_doLoadBytes = 44;
const uint32_t avmplus_Domain_getClass = 45;
const uint32_t avmplus_Domain_getVariables = 46;
const uint32_t avmplus_Domain_getClassName = 47;
const uint32_t avmplus_Domain_isAssignableAs = 48;
const uint32_t avmplus_Domain_isDynamic = 49;
const uint32_t avmplus_Domain_domainMemory_get = 50;
const uint32_t avmplus_Domain_domainMemory_set = 51;
const uint32_t flash_utils_Dictionary_private_init = 249;
const uint32_t avmplus_System_private_doExit = 590;
const uint32_t avmplus_System_getAvmplusVersion = 591;
const uint32_t avmplus_System_trace = 592;
const uint32_t avmplus_System_getTimer = 593;
const uint32_t avmplus_System_private_getArgv = 594;
const uint32_t flash_utils_ByteArray_readBytes = 651;
const uint32_t flash_utils_ByteArray_writeBytes = 652;
const uint32_t flash_utils_ByteArray_writeBoolean = 653;
const uint32_t flash_utils_ByteArray_writeByte = 654;
const uint32_t flash_utils_ByteArray_writeShort = 655;
const uint32_t flash_utils_ByteArray_writeInt = 656;
const uint32_t flash_utils_ByteArray_writeUnsignedInt = 657;
const uint32_t flash_utils_ByteArray_writeFloat = 658;
const uint32_t flash_utils_ByteArray_writeDouble = 659;
const uint32_t flash_utils_ByteArray_writeUTF = 660;
const uint32_t flash_utils_ByteArray_writeUTFBytes = 661;
const uint32_t flash_utils_ByteArray_readBoolean = 662;
const uint32_t flash_utils_ByteArray_readByte = 663;
const uint32_t flash_utils_ByteArray_readUnsignedByte = 664;
const uint32_t flash_utils_ByteArray_readShort = 665;
const uint32_t flash_utils_ByteArray_readUnsignedShort = 666;
const uint32_t flash_utils_ByteArray_readInt = 667;
const uint32_t flash_utils_ByteArray_readUnsignedInt = 668;
const uint32_t flash_utils_ByteArray_readFloat = 669;
const uint32_t flash_utils_ByteArray_readDouble = 670;
const uint32_t flash_utils_ByteArray_readUTF = 671;
const uint32_t flash_utils_ByteArray_readUTFBytes = 672;
const uint32_t flash_utils_ByteArray_length_get = 673;
const uint32_t flash_utils_ByteArray_length_set = 674;
const uint32_t flash_utils_ByteArray_private_zlib_compress = 675;
const uint32_t flash_utils_ByteArray_private_zlib_uncompress = 677;
const uint32_t flash_utils_ByteArray_private__toString = 679;
const uint32_t flash_utils_ByteArray_bytesAvailable_get = 681;
const uint32_t flash_utils_ByteArray_position_get = 682;
const uint32_t flash_utils_ByteArray_position_set = 683;
const uint32_t flash_utils_ByteArray_endian_get = 684;
const uint32_t flash_utils_ByteArray_endian_set = 685;
const uint32_t flash_net_Socket_private_nb_disconnect = 792;
const uint32_t flash_net_Socket_private_nb_connect = 793;
const uint32_t flash_net_Socket_private_nb_read = 794;
const uint32_t flash_net_Socket_private_nb_write = 795;

extern double thanelib_d2d_o_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_ByteArray_readFloat_thunk  thanelib_d2d_o_thunk
#define flash_utils_ByteArray_readDouble_thunk  thanelib_d2d_o_thunk

extern AvmBox thanelib_v2a_oouu_opti0_opti0_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_ByteArray_writeBytes_thunk  thanelib_v2a_oouu_opti0_opti0_thunk
#define flash_utils_ByteArray_readBytes_thunk  thanelib_v2a_oouu_opti0_opti0_thunk

extern AvmBox thanelib_a2a_os_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_Domain_getClass_thunk  thanelib_a2a_os_thunk

extern AvmBox thanelib_a2a_oo_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_Domain_domainMemory_set_thunk  thanelib_a2a_oo_thunk
#define avmplus_Domain_private_doLoadBytes_thunk  thanelib_a2a_oo_thunk

extern AvmBox thanelib_s2a_o_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_System_getAvmplusVersion_thunk  thanelib_s2a_o_thunk
#define flash_utils_ByteArray_readUTF_thunk  thanelib_s2a_o_thunk
#define flash_utils_ByteArray_private__toString_thunk  thanelib_s2a_o_thunk
#define flash_utils_ByteArray_endian_get_thunk  thanelib_s2a_o_thunk

extern AvmBox thanelib_a2a_oa_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_Domain_getVariables_thunk  thanelib_a2a_oa_thunk

extern AvmBox thanelib_u2a_o_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_ByteArray_readUnsignedShort_thunk  thanelib_u2a_o_thunk
#define flash_utils_ByteArray_bytesAvailable_get_thunk  thanelib_u2a_o_thunk
#define flash_utils_ByteArray_length_get_thunk  thanelib_u2a_o_thunk
#define flash_utils_ByteArray_readUnsignedInt_thunk  thanelib_u2a_o_thunk
#define flash_utils_ByteArray_readUnsignedByte_thunk  thanelib_u2a_o_thunk
#define avmplus_Domain_MIN_DOMAIN_MEMORY_LENGTH_get_thunk  thanelib_u2a_o_thunk
#define flash_utils_ByteArray_position_get_thunk  thanelib_u2a_o_thunk

extern AvmBox thanelib_b2a_ooa_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_Domain_isAssignableAs_thunk  thanelib_b2a_ooa_thunk

extern AvmBox thanelib_i2a_o_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_System_getTimer_thunk  thanelib_i2a_o_thunk
#define flash_utils_ByteArray_readInt_thunk  thanelib_i2a_o_thunk
#define flash_utils_ByteArray_readShort_thunk  thanelib_i2a_o_thunk
#define flash_utils_ByteArray_readByte_thunk  thanelib_i2a_o_thunk

extern AvmBox thanelib_v2a_oso_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_System_trace_thunk  thanelib_v2a_oso_thunk

extern AvmBox thanelib_b2a_oa_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_Domain_isDynamic_thunk  thanelib_b2a_oa_thunk

extern AvmBox thanelib_s2a_ou_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_ByteArray_readUTFBytes_thunk  thanelib_s2a_ou_thunk

extern AvmBox thanelib_s2a_oa_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_Domain_getClassName_thunk  thanelib_s2a_oa_thunk

extern AvmBox thanelib_i2a_oo_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_net_Socket_private_nb_read_thunk  thanelib_i2a_oo_thunk
#define flash_net_Socket_private_nb_write_thunk  thanelib_i2a_oo_thunk

extern AvmBox thanelib_v2a_od_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_ByteArray_writeFloat_thunk  thanelib_v2a_od_thunk
#define flash_utils_ByteArray_writeDouble_thunk  thanelib_v2a_od_thunk

extern AvmBox thanelib_v2a_ob_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_ByteArray_writeBoolean_thunk  thanelib_v2a_ob_thunk
#define flash_utils_Dictionary_private_init_thunk  thanelib_v2a_ob_thunk

extern AvmBox thanelib_a2a_o_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define avmplus_System_private_getArgv_thunk  thanelib_a2a_o_thunk
#define avmplus_Domain_currentDomain_get_thunk  thanelib_a2a_o_thunk
#define avmplus_Domain_domainMemory_get_thunk  thanelib_a2a_o_thunk

extern AvmBox thanelib_v2a_o_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_ByteArray_private_zlib_compress_thunk  thanelib_v2a_o_thunk
#define flash_net_Socket_private_nb_disconnect_thunk  thanelib_v2a_o_thunk
#define avmplus_Domain_private_initNewDomain_thunk  thanelib_v2a_o_thunk
#define flash_utils_ByteArray_private_zlib_uncompress_thunk  thanelib_v2a_o_thunk

extern AvmBox thanelib_v2a_ou_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_ByteArray_position_set_thunk  thanelib_v2a_ou_thunk
#define flash_utils_ByteArray_length_set_thunk  thanelib_v2a_ou_thunk
#define flash_utils_ByteArray_writeUnsignedInt_thunk  thanelib_v2a_ou_thunk

extern AvmBox thanelib_b2a_o_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_ByteArray_readBoolean_thunk  thanelib_b2a_o_thunk

extern AvmBox thanelib_v2a_os_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_ByteArray_writeUTF_thunk  thanelib_v2a_os_thunk
#define flash_utils_ByteArray_endian_set_thunk  thanelib_v2a_os_thunk
#define flash_utils_ByteArray_writeUTFBytes_thunk  thanelib_v2a_os_thunk

extern AvmBox thanelib_v2a_oi_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_utils_ByteArray_writeByte_thunk  thanelib_v2a_oi_thunk
#define flash_utils_ByteArray_writeInt_thunk  thanelib_v2a_oi_thunk
#define flash_utils_ByteArray_writeShort_thunk  thanelib_v2a_oi_thunk
#define avmplus_System_private_doExit_thunk  thanelib_v2a_oi_thunk

extern AvmBox thanelib_i2a_osi_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
#define flash_net_Socket_private_nb_connect_thunk  thanelib_i2a_osi_thunk

