/*! \file response_status_code.h
 *  \brief status codes for responses.
 */


#pragma once
/*!
 * @brief A struct to hold all status codes which are needed
 * \enum response_status_code
 */
enum response_status_code {
    STATUS_200_OK = 200,                    /**< valid request */
    STATUS_201_CREATED = 201,               /**< unused*/
    STATUS_202_ACCEPTED = 202,              /**< unused*/
    STATUS_204_NO_CONTENT = 204,            /**< unused*/

    STATUS_301_MOVED_PERMANENTLY = 301,     /**< unused*/
    STATUS_302_MOVED_TEMPORARILY = 302,     /**< unused*/
    STATUS_304_NOT_MODIFIED = 304,          /**< unused*/

    STATUS_400_BAD_REQUEST = 400,           /**< malformed*/
    STATUS_401_UNAUTHORIZED = 401,          /**< authorisation failed or in progress*/
    STATUS_403_FORBIDDEN = 403,             /**< unused*/
    STATUS_404_NOT_FOUND = 404,             /**< File not found*/

    STATUS_500_INTERNAL_SERVER_ERROR = 500, /**< Server Error*/
    STATUS_501_NOT_IMPLEMENTED = 501,       /**< other Method than get and a valid request*/
    STATUS_502_BAD_GATEWAY = 502,           /**< unused*/
    STATUS_503_SERVICE_UNAVAILABLE = 503    /**< unused*/
};
