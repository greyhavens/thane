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
const uint32_t abcclass_AMF3 = 0;
const uint32_t abcclass_AMF3Decoder = 1;
const uint32_t abcclass_private_DecoderContext = 2;
const uint32_t abcclass_private_Traits = 3;
const uint32_t abcclass_AMF3Encoder = 4;
const uint32_t abcclass_private_EncoderContext = 5;
const uint32_t abcclass_private_References = 6;
const uint32_t abcclass_avmplus_Domain = 7;
const uint32_t abcclass_com_adobe_errors_IllegalStateError = 8;
const uint32_t abcclass_com_adobe_net_IURIResolver = 9;
const uint32_t abcclass_com_adobe_net_MimeTypeMap = 10;
const uint32_t abcclass_com_adobe_net_URI = 11;
const uint32_t abcclass_com_adobe_serialization_json_JSON = 12;
const uint32_t abcclass_com_adobe_serialization_json_JSONDecoder = 13;
const uint32_t abcclass_com_adobe_serialization_json_JSONEncoder = 14;
const uint32_t abcclass_com_adobe_serialization_json_JSONParseError = 15;
const uint32_t abcclass_com_adobe_serialization_json_JSONToken = 16;
const uint32_t abcclass_com_adobe_serialization_json_JSONTokenizer = 17;
const uint32_t abcclass_com_adobe_serialization_json_JSONTokenType = 18;
const uint32_t abcclass_com_adobe_utils_ArrayUtil = 19;
const uint32_t abcclass_com_adobe_utils_DictionaryUtil = 20;
const uint32_t abcclass_com_adobe_utils_IntUtil = 21;
const uint32_t abcclass_com_adobe_utils_NumberFormatter = 22;
const uint32_t abcclass_com_adobe_utils_StringUtil = 23;
const uint32_t abcclass_com_adobe_utils_XMLUtil = 24;
const uint32_t abcclass_de_polygonal_ds_Collection = 25;
const uint32_t abcclass_de_polygonal_ds_Iterator = 26;
const uint32_t abcclass_flash_display_StageQuality = 27;
const uint32_t abcclass_flash_errors_EOFError = 28;
const uint32_t abcclass_flash_errors_IllegalOperationError = 29;
const uint32_t abcclass_flash_errors_IOError = 30;
const uint32_t abcclass_flash_errors_MemoryError = 31;
const uint32_t abcclass_flash_events_Event = 32;
const uint32_t abcclass_flash_events_IEventDispatcher = 33;
const uint32_t abcclass_flash_geom_Matrix = 34;
const uint32_t abcclass_flash_geom_Point = 35;
const uint32_t abcclass_flash_geom_Rectangle = 36;
const uint32_t abcclass_flash_net_ObjectEncoding = 37;
const uint32_t abcclass_flash_net_URLLoaderDataFormat = 38;
const uint32_t abcclass_flash_net_URLRequest = 39;
const uint32_t abcclass_flash_utils_Dictionary = 40;
const uint32_t abcclass_flash_utils_Endian = 41;
const uint32_t abcclass_flash_utils_IDataInput = 42;
const uint32_t abcclass_flash_utils_IDataOutput = 43;
const uint32_t abcclass_flash_utils_IExternalizable = 44;
const uint32_t abcclass_mx_core_IChildList = 45;
const uint32_t abcclass_mx_core_IRawChildrenContainer = 46;
const uint32_t abcclass_mx_resources_IResourceBundle = 47;
const uint32_t abcclass_mx_resources_IResourceManager = 48;
const uint32_t abcclass_mx_resources_Locale = 49;
const uint32_t abcclass_mx_resources_ResourceManager = 50;
const uint32_t abcclass_org_httpclient_events_HttpListener = 51;
const uint32_t abcclass_org_httpclient_http_multipart_Multipart = 52;
const uint32_t abcclass_org_httpclient_http_multipart_Part = 53;
const uint32_t abcclass_org_httpclient_HttpHeader = 54;
const uint32_t abcclass_org_httpclient_HttpRequest = 55;
const uint32_t abcclass_org_httpclient_HttpResponse = 56;
const uint32_t abcclass_org_httpclient_HttpSocket = 57;
const uint32_t abcclass_org_httpclient_HttpTimer = 58;
const uint32_t abcclass_org_httpclient_io_HttpBuffer = 59;
const uint32_t abcclass_org_httpclient_io_HttpRequestBuffer = 60;
const uint32_t abcclass_org_httpclient_io_HttpResponseBuffer = 61;
const uint32_t abcclass_org_httpclient_Log = 62;
const uint32_t abcclass_Thane = 63;
const uint32_t abcclass_private_SpawnedDomain = 64;
const uint32_t abcclass_Thanette = 65;
const uint32_t abcclass_avmplus_System = 66;
const uint32_t abcclass_flash_system_Capabilities = 67;
const uint32_t abcclass_flash_system_System = 68;
const uint32_t abcclass_de_polygonal_ds_Heap = 69;
const uint32_t abcclass_private_HeapIterator = 70;
const uint32_t abcclass_flash_events_KeyboardEvent = 71;
const uint32_t abcclass_flash_events_MouseEvent = 72;
const uint32_t abcclass_flash_events_NetStatusEvent = 73;
const uint32_t abcclass_flash_events_ProgressEvent = 74;
const uint32_t abcclass_flash_events_TextEvent = 75;
const uint32_t abcclass_flash_events_TimerEvent = 76;
const uint32_t abcclass_org_httpclient_events_HttpDataEvent = 77;
const uint32_t abcclass_org_httpclient_events_HttpStatusEvent = 78;
const uint32_t abcclass_TraceEvent = 79;
const uint32_t abcclass_flash_events_EventDispatcher = 80;
const uint32_t abcclass_flash_utils_ByteArray = 81;
const uint32_t abcclass_org_httpclient_http_Delete = 82;
const uint32_t abcclass_org_httpclient_http_Get = 83;
const uint32_t abcclass_org_httpclient_http_Head = 84;
const uint32_t abcclass_org_httpclient_http_Post = 85;
const uint32_t abcclass_org_httpclient_http_Put = 86;
const uint32_t abcclass_flash_events_ErrorEvent = 87;
const uint32_t abcclass_flash_display_DisplayObject = 88;
const uint32_t abcclass_flash_display_LoaderInfo = 89;
const uint32_t abcclass_flash_net_Socket = 90;
const uint32_t abcclass_flash_net_URLLoader = 91;
const uint32_t abcclass_flash_utils_Timer = 92;
const uint32_t abcclass_private_Buddy = 93;
const uint32_t abcclass_org_httpclient_HttpClient = 94;
const uint32_t abcclass_com_adobe_net_URIEncodingBitmap = 95;
const uint32_t abcclass_flash_events_HTTPStatusEvent = 96;
const uint32_t abcclass_flash_events_IOErrorEvent = 97;
const uint32_t abcclass_flash_events_SecurityErrorEvent = 98;
const uint32_t abcclass_org_httpclient_events_HttpErrorEvent = 99;
const uint32_t abcclass_flash_display_DisplayObjectContainer = 100;
const uint32_t abcclass_com_adobe_net_proxies_RFC2817Socket = 101;

/* methods */
const uint32_t avmplus_Domain_currentDomain_get = 50;
const uint32_t avmplus_Domain_MIN_DOMAIN_MEMORY_LENGTH_get = 51;
const uint32_t avmplus_Domain_private_initNewDomain = 53;
const uint32_t avmplus_Domain_private_doLoadBytes = 55;
const uint32_t avmplus_Domain_getClass = 56;
const uint32_t avmplus_Domain_getVariables = 57;
const uint32_t avmplus_Domain_getClassName = 58;
const uint32_t avmplus_Domain_isAssignableAs = 59;
const uint32_t avmplus_Domain_isDynamic = 60;
const uint32_t avmplus_Domain_domainMemory_get = 61;
const uint32_t avmplus_Domain_domainMemory_set = 62;
const uint32_t flash_utils_Dictionary_private_init = 359;
const uint32_t avmplus_System_private_doExit = 589;
const uint32_t avmplus_System_getAvmplusVersion = 590;
const uint32_t avmplus_System_trace = 591;
const uint32_t avmplus_System_getTimer = 592;
const uint32_t avmplus_System_private_getArgv = 593;
const uint32_t flash_utils_ByteArray_readBytes = 670;
const uint32_t flash_utils_ByteArray_writeBytes = 671;
const uint32_t flash_utils_ByteArray_writeBoolean = 672;
const uint32_t flash_utils_ByteArray_writeByte = 673;
const uint32_t flash_utils_ByteArray_writeShort = 674;
const uint32_t flash_utils_ByteArray_writeInt = 675;
const uint32_t flash_utils_ByteArray_writeUnsignedInt = 676;
const uint32_t flash_utils_ByteArray_writeFloat = 677;
const uint32_t flash_utils_ByteArray_writeDouble = 678;
const uint32_t flash_utils_ByteArray_writeUTF = 679;
const uint32_t flash_utils_ByteArray_writeUTFBytes = 680;
const uint32_t flash_utils_ByteArray_readBoolean = 681;
const uint32_t flash_utils_ByteArray_readByte = 682;
const uint32_t flash_utils_ByteArray_readUnsignedByte = 683;
const uint32_t flash_utils_ByteArray_readShort = 684;
const uint32_t flash_utils_ByteArray_readUnsignedShort = 685;
const uint32_t flash_utils_ByteArray_readInt = 686;
const uint32_t flash_utils_ByteArray_readUnsignedInt = 687;
const uint32_t flash_utils_ByteArray_readFloat = 688;
const uint32_t flash_utils_ByteArray_readDouble = 689;
const uint32_t flash_utils_ByteArray_readUTF = 690;
const uint32_t flash_utils_ByteArray_readUTFBytes = 691;
const uint32_t flash_utils_ByteArray_length_get = 692;
const uint32_t flash_utils_ByteArray_length_set = 693;
const uint32_t flash_utils_ByteArray_private_zlib_compress = 694;
const uint32_t flash_utils_ByteArray_private_zlib_uncompress = 696;
const uint32_t flash_utils_ByteArray_private__toString = 698;
const uint32_t flash_utils_ByteArray_bytesAvailable_get = 700;
const uint32_t flash_utils_ByteArray_position_get = 701;
const uint32_t flash_utils_ByteArray_position_set = 702;
const uint32_t flash_utils_ByteArray_endian_get = 703;
const uint32_t flash_utils_ByteArray_endian_set = 704;
const uint32_t flash_net_Socket_private_nb_disconnect = 756;
const uint32_t flash_net_Socket_private_nb_connect = 757;
const uint32_t flash_net_Socket_private_nb_read = 758;
const uint32_t flash_net_Socket_private_nb_write = 759;

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

