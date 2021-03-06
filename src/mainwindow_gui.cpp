#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "operators.h"
#include "bannedplayers.h"
#include "serverproperties.h"
#include "servermods.h"
#include <QListWidgetItem>
#include <QStringList>
#include <QDebug>


void MainWindow::gui_log_print_info(QString str)
{
    ui->console_area->moveCursor(QTextCursor::End);
    ui->console_area->insertHtml(QString().append(blueFont).append(str).append(endFont));
    ui->console_area->append("");
}

void MainWindow::gui_log_print_warn(QString str)
{
    ui->console_area->moveCursor(QTextCursor::End);
    ui->console_area->insertHtml(QString().append(yellowFont).append(str).append(endFont));
    ui->console_area->append("");
}

void MainWindow::gui_log_print_error(QString str)
{
    ui->console_area->moveCursor(QTextCursor::End);
    ui->console_area->insertHtml(QString().append(redFont).append(str).append(endFont));
    ui->console_area->append("");
}

void MainWindow::gui_server_stoped()
{
    processing_thread.stop();
    ui->start_button->setEnabled(true);
    ui->jar_combo->setEnabled(true);
    ui->stop_button->setEnabled(false);
    ui->kill_button->setEnabled(false);
    ui->status_label->setText(QString ("Not running"));
    ui->status_icon->setPixmap(stopped_icon);
    ui->worlds_update_button->setEnabled(true);
    ui->properties_update_button->setEnabled(true);
    ui->properties_save_button->setEnabled(true);
    ui->working_dir_choose_button->setEnabled(true);
    for (int i = 0; i < ui->online_players_list->count(); i++) delete ui->online_players_list->item(0);//clearing online players list
}

void MainWindow::gui_server_starting()
{
    processing_thread.start();
    ui->start_button->setEnabled(false);
    ui->jar_combo->setEnabled(false);
    ui->stop_button->setEnabled(true);
    ui->kill_button->setEnabled(true);
    ui->status_label->setText(QString ("Starting"));
    ui->status_icon->setPixmap(starting_icon);
    ui->worlds_update_button->setEnabled(false);
    ui->properties_update_button->setEnabled(false);
    ui->properties_save_button->setEnabled(false);
    ui->working_dir_choose_button->setEnabled(false);

}

void MainWindow::gui_server_running()
{
    ui->status_label->setText("Running");
    ui->status_icon->setPixmap(running_icon);
}





//players tab
void MainWindow::gui_make_player_online (QString player_name)
{
    if (Settings::get_instance()->get_play_sound()) join_sound.play();
    QString face;
    if (Operators::getInstance()->is_operator(player_name)) face = QString(":/images/face_operator.png");
    else face = QString(":/images/face.png");

    QListWidgetItem* item = new QListWidgetItem(QPixmap(face), player_name, ui->online_players_list);
}

void MainWindow::gui_make_player_offline (QString player_name)
{
    delete find_item(ui->online_players_list, player_name);
}

void MainWindow::gui_make_player_opped (QString player_name)
{
    Operators::getInstance()->add_operator(player_name);
    QListWidgetItem* item = find_item (ui->online_players_list, player_name);
    if (item != nullptr) item->setIcon(QPixmap(QString(":/images/face_operator.png")));
}

void MainWindow::gui_make_player_deopped (QString player_name)
{
    Operators::getInstance()->remove_operator(player_name);
    QListWidgetItem* item = find_item (ui->online_players_list, player_name);
    if (item != nullptr) item->setIcon(QPixmap(QString(":/images/face.png")));
}

void MainWindow::gui_make_player_banned(QString player_name)
{
    QListWidgetItem* item = new QListWidgetItem(QPixmap(QString(":/images/face_banned.png")), player_name, ui->banned_players_list);
}

void MainWindow::gui_make_player_unbanned(QString player_name)
{
    delete find_item(ui->banned_players_list, player_name);
}

void MainWindow::gui_load_banned_players()
{
    ui->banned_players_list->clear();
    for (int i = 0; i < BannedPlayers::get_instance()->size(); i++) gui_make_player_banned(BannedPlayers::get_instance()->at(i));
}





//worlds tab
void MainWindow::gui_load_worlds()
{
    const QStringList* worlds = Settings::get_instance()->get_available_worlds();
    ui->worlds_list->clear();
    for (int i = 0; i < worlds->size(); i++)
    {
        QString icon;
        if (!worlds->at(i).compare(ServerProperties::get_instance()->get_level_name())) icon = QString (":/images/world_32_tick.png");
        else icon = QString (":/images/world_32.png");
        QListWidgetItem* item = new QListWidgetItem(QPixmap(icon), worlds->at(i), ui->worlds_list);
    }
}





//mods tab
void MainWindow::gui_load_mods()
{
    ui->mods_list->clear();

    QList<QString> mods_filenames = ServerMods::get_instance()->get_mods_filenames();
    for (int i = 0; i < mods_filenames.size(); i++)
    {
        QListWidgetItem* item = new QListWidgetItem(mods_filenames.at(i), ui->mods_list);
    }
}





//properties tab
void MainWindow::gui_load_server_properties()
{
    ui->gen_sets_edit->setText (ServerProperties::get_instance()->get_gen_sets());
    ui->motd_field->setText (ServerProperties::get_instance()->get_motd());
    ui->op_level_combo->setCurrentIndex (ServerProperties::get_instance()->get_op_level()-1);
    if (ServerProperties::get_instance()->get_broadcast_console()) ui->broadcast_console_true->setChecked(true);
    else ui->broadcast_console_false->setChecked(true);
    if (ServerProperties::get_instance()->get_nether()) ui->nether_true->setChecked(true);
    else ui->nether_false->setChecked(true);
    if (ServerProperties::get_instance()->get_query()) ui->query_true->setChecked(true);
    else ui->query_false->setChecked(true);
    ui->query_port_spin->setValue(ServerProperties::get_instance()->get_query_port());
    if (ServerProperties::get_instance()->get_flight()) ui->flight_true->setChecked(true);
    else ui->flight_false->setChecked(true);
    ui->port_spin->setValue(ServerProperties::get_instance()->get_port());
    ui->world_size_spin->setValue(ServerProperties::get_instance()->get_world_size());
    ui->level_type_combo->setCurrentText(ServerProperties::get_instance()->get_level_type());
    if (ServerProperties::get_instance()->get_rcon()) ui->rcon_true->setChecked(true);
    else ui->rcon_false->setChecked(true);
    ui->rcon_port_spin->setValue(ServerProperties::get_instance()->get_rcon_port());
    ui->rcon_password_field->setText(ServerProperties::get_instance()->get_rcon_password());
    if (ServerProperties::get_instance()->get_force_game_mode()) ui->force_game_mode_true->setChecked(true);
    else ui->force_game_mode_false->setChecked(true);
    ui->seed_field->setText(ServerProperties::get_instance()->get_seed());
    ui->ip_field->setText(ServerProperties::get_instance()->get_ip());
    ui->build_height_spin->setValue(ServerProperties::get_instance()->get_build_height());
    ui->view_distance_spin->setValue(ServerProperties::get_instance()->get_view_distance());
    if (ServerProperties::get_instance()->get_npc()) ui->npc_true->setChecked(true);
    else ui->npc_false->setChecked(true);
    if (ServerProperties::get_instance()->get_whitelist()) ui->whitelist_true->setChecked(true);
    else ui->whitelist_false->setChecked(true);
    if (ServerProperties::get_instance()->get_animals()) ui->animals_true->setChecked(true);
    else ui->animals_false->setChecked(true);
    if (ServerProperties::get_instance()->get_monsters()) ui->monsters_true->setChecked(true);
    else ui->monsters_false->setChecked(true);
    if (ServerProperties::get_instance()->get_generate_structs()) ui->generate_structs_true->setChecked(true);
    else ui->generate_structs_false->setChecked(true);
    if (ServerProperties::get_instance()->get_snooper()) ui->snooper_true->setChecked(true);
    else ui->snooper_false->setChecked(true);
    if (ServerProperties::get_instance()->get_hardcore()) ui->hardcore_true->setChecked(true);
    else ui->hardcore_false->setChecked(true);
    if (ServerProperties::get_instance()->get_online_mode()) ui->online_mode_true->setChecked(true);
    else ui->online_mode_false->setChecked(true);
    if (ServerProperties::get_instance()->get_prevent_proxy()) ui->prevent_proxy_true->setChecked(true);
    else ui->prevent_proxy_false->setChecked(true);
    ui->resourcepack_field->setText(ServerProperties::get_instance()->get_resourcepack());
    ui->resourcepack_sha_1_field->setText(ServerProperties::get_instance()->get_resourcepack_sha_1());
    if (ServerProperties::get_instance()->get_pvp()) ui->pvp_true->setChecked(true);
    else ui->pvp_false->setChecked(true);
    ui->difficulty_combo->setCurrentIndex(ServerProperties::get_instance()->get_difficulty());
    if (ServerProperties::get_instance()->get_command_block()) ui->command_block_true->setChecked(true);
    else ui->command_block_false->setChecked(true);
    ui->idle_timeout_spin->setValue(ServerProperties::get_instance()->get_idle_timeout());
    ui->game_mode_combo->setCurrentIndex(ServerProperties::get_instance()->get_game_mode());
    ui->network_compression_spin->setValue(ServerProperties::get_instance()->get_network_compression());
    ui->max_players_spin->setValue(ServerProperties::get_instance()->get_max_players());
    ui->tick_time_spin->setValue(ServerProperties::get_instance()->get_tick_time());
}

void MainWindow::gui_save_server_properties()
{
    ServerProperties::get_instance()->set_gen_sets(ui->gen_sets_edit->text());
    ServerProperties::get_instance()->set_motd(ui->motd_field->text());
    ServerProperties::get_instance()->set_op_level(ui->op_level_combo->currentIndex()+1);
    ServerProperties::get_instance()->set_broadcast_console(ui->broadcast_console_true->isChecked());
    ServerProperties::get_instance()->set_nether(ui->nether_true->isChecked());
    ServerProperties::get_instance()->set_query(ui->query_true->isChecked());
    ServerProperties::get_instance()->set_query_port(ui->query_port_spin->value());
    ServerProperties::get_instance()->set_flight(ui->flight_true->isChecked());
    ServerProperties::get_instance()->set_port(ui->port_spin->value());
    ServerProperties::get_instance()->set_world_size(ui->world_size_spin->value());
    ServerProperties::get_instance()->set_level_type(ui->level_type_combo->currentText());
    ServerProperties::get_instance()->set_rcon(ui->rcon_true->isChecked());
    ServerProperties::get_instance()->set_rcon_port(ui->rcon_port_spin->value());
    ServerProperties::get_instance()->set_rcon_password(ui->rcon_password_field->text());
    ServerProperties::get_instance()->set_force_game_mode(ui->force_game_mode_true->isChecked());
    ServerProperties::get_instance()->set_seed(ui->seed_field->text());
    if (ui->ip_field->text().length() > 3) ServerProperties::get_instance()->set_ip(ui->ip_field->text());
    else ServerProperties::get_instance()->set_ip (QString (""));
    ServerProperties::get_instance()->set_build_height(ui->build_height_spin->value());
    ServerProperties::get_instance()->set_view_distance(ui->view_distance_spin->value());
    ServerProperties::get_instance()->set_npc(ui->npc_true->isChecked());
    ServerProperties::get_instance()->set_whitelist(ui->whitelist_true->isChecked());
    ServerProperties::get_instance()->set_animals(ui->animals_true->isChecked());
    ServerProperties::get_instance()->set_monsters(ui->monsters_true->isChecked());
    ServerProperties::get_instance()->set_generate_structs(ui->generate_structs_true->isChecked());
    ServerProperties::get_instance()->set_snooper(ui->snooper_true->isChecked());
    ServerProperties::get_instance()->set_hardcore(ui->hardcore_true->isChecked());
    ServerProperties::get_instance()->set_online_mode(ui->online_mode_true->isChecked());
    ServerProperties::get_instance()->set_prevent_proxy(ui->prevent_proxy_true->isChecked());
    ServerProperties::get_instance()->set_resourcepack(ui->resourcepack_field->text());
    ServerProperties::get_instance()->set_resourcepack_sha_1(ui->resourcepack_sha_1_field->text());
    ServerProperties::get_instance()->set_pvp(ui->pvp_true->isChecked());
    ServerProperties::get_instance()->set_difficulty(ui->difficulty_combo->currentIndex());
    ServerProperties::get_instance()->set_command_block(ui->command_block_true->isChecked());
    ServerProperties::get_instance()->set_idle_timeout(ui->idle_timeout_spin->value());
    ServerProperties::get_instance()->set_game_mode(ui->game_mode_combo->currentIndex());
    ServerProperties::get_instance()->set_network_compression(ui->network_compression_spin->value());
    ServerProperties::get_instance()->set_max_players(ui->max_players_spin->value());
    ServerProperties::get_instance()->set_tick_time(ui->tick_time_spin->value());
}





//settings tab
void MainWindow::gui_update_jars()
{
    QString current_jar = Settings::get_instance()->get_jar();

    const QStringList* available_jars = Settings::get_instance()->get_available_jars();
    ui->jar_combo->clear();
    for (int i = 0; i < available_jars->size(); i++)
    {
        ui->jar_combo->addItem(available_jars->at(i), available_jars->at(i));
    }

    int index = ui->jar_combo->findData(current_jar);
    if (index != -1 ) ui->jar_combo->setCurrentIndex(index);
    else              ui->jar_combo->setCurrentIndex(0);
}

void MainWindow::gui_load_settings()
{
    ui->working_dir_field->setText(Settings::get_instance()->get_working_dir());
    ui->ram_slider->setValue(Settings::get_instance()->get_ram());

    switch (Settings::get_instance()->get_log_level()) {
    case Settings::LogLevel::info:
        ui->log_level_box->setCurrentText("Info");
        break;
    case Settings::LogLevel::warn:
        ui->log_level_box->setCurrentText("Warning");
        break;
    case Settings::LogLevel::error:
        ui->log_level_box->setCurrentText("Error");
        break;
    default:
        break;
    }
    ui->play_sound_checkbox->setChecked(Settings::get_instance()->get_play_sound());
}
























