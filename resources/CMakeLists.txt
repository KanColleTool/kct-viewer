if(UNIX AND NOT APPLE)
	install(FILES "KCTViewer.desktop" DESTINATION "share/applications" RENAME "kancolletool-viewer.desktop")
	install(FILES "KCTViewer.png" DESTINATION "share/icons/hicolor/192x192/apps" RENAME "kancolletool-viewer.png")
endif()
